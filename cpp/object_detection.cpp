/* Copyright 2021 iwatake2222
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
    http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/
/*** Include ***/
/* for general */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <cstdint>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <numeric>
#include <sstream>

#include <bitset>

#include <iostream>

/* for OpenCV */
#include <opencv2/opencv.hpp>

/* for My modules */
#include "common_helper.h"
#include "common_helper_cv.h"
#include "object_detection.h"
#include "object_detection_position.h"
#include "position_status.h"
#include "basic_message.h"
#include "send_message.h"

/* for Protocol Buffers */
#include "SendData.pb.h"

/*** Macro ***/
#define TAG "ObjectDetection"
#define PRINT(...) COMMON_HELPER_PRINT(TAG, __VA_ARGS__)
#define PRINT_E(...) COMMON_HELPER_PRINT_E(TAG, __VA_ARGS__)

/*** Function ***/
int32_t ObjectDetection::Initialize(const std::string &work_dir, const int32_t num_threads)
{
    if (detection_engine_.Initialize(work_dir, num_threads) != DetectionEngine::kRetOk)
    {
        detection_engine_.Finalize();
        return kRetErr;
    }

    coordinate_.AreaCoordinateIni();
    return kRetOk;
}

int32_t ObjectDetection::Finalize()
{
    if (detection_engine_.Finalize() != DetectionEngine::kRetOk)
    {
        return kRetErr;
    }
    return kRetOk;
}

int32_t ObjectDetection::Process(const cv::Mat &mat, const cv::Mat &mat_transform)
{
    /* Run inference to detect objects */
    DetectionEngine::Result det_result;
    if (detection_engine_.Process(mat, det_result) != DetectionEngine::kRetOk)
    {
        return kRetErr;
    }
    roi_ = cv::Rect(det_result.crop.x, det_result.crop.y, det_result.crop.w, det_result.crop.h);
    time_pre_process_ = det_result.time_pre_process;
    time_inference_ = det_result.time_inference;
    time_post_process_ = det_result.time_post_process;

    /* Track */
    tracker_.Update(det_result.bbox_list);
    auto &track_list = tracker_.GetTrackList();

    return kRetOk;
}

ObjectDetection::Send_Data_Raw ObjectDetection::SendImgCreate(cv::Mat mat)
{

    Send_Data_Raw SendDataRaw;
    // std::vector<unsigned char> SendImg;
    auto &track_list = tracker_.GetTrackList();
    auto tmp_time = GetSendTime();
    auto times = position_status_.TimeConversion(tmp_time);

    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(localtime(&now_c), "%Y%m%d_%H%M%S");

    // std::cout << ss.str() << std::endl;

    cv::cvtColor(mat, mat, cv::COLOR_RGB2GRAY);
    cv::imwrite("test.jpg", mat);
    SendDataRaw.Img = mat;

    std::vector<int> param = std::vector<int>(2);
    // jpg : cv::IMWRITE_JPEG_QUALITY
    // png : cv::IMWRITE_PNG_COMPRESSION
    param[0] = cv::IMWRITE_JPEG_QUALITY;
    //エンコードの画質 : 0 ~ 100
    param[1] = 100;

    SendDataRaw.TimeStamp = ss.str();
    SendDataRaw.Count = track_list.size();

    // std::ofstream ofs("test.txt", std::ios::out);
    // ofs << track_list.size() << std::endl;
    // SendImg.push_back(track_list.size());
    // std::cout << "画像+タイムスタンプ+物標数 " << SendImg.size() << std::endl;
    int32_t det_num = 0;
    for (auto &track : track_list)
    {
        if (track.GetUndetectedCount() > 0)
            continue;
        const auto &bbox = track.GetLatestData().bbox_raw;

        std::string name = bbox.label;
        int labelId = 15;
        if (name == "bicycle")
        {
            labelId = 4;
        }
        if (name == "car")
        {
            labelId = 2;
        }
        if (name == "elecar")
        {
            labelId = 5;
        }
        if (name == "lift")
        {
            labelId = 2;
        }
        if (name == "person")
        {
            labelId = 6;
        }
        if (name == "tagnova")
        {
            labelId = 5;
        }
        if (name == "truck")
        {
            labelId = 0;
        }

        // std::ofstream ofs("test.txt", std::ios::app);
        // ofs << std::fixed << labelId << " ";
        // ofs << std::fixed << bbox.x << " ";
        // ofs << std::fixed << bbox.y << " ";
        // ofs << std::fixed << bbox.w << " ";
        // ofs << std::fixed << bbox.h;
        // ofs << std::endl;
        // SendImg.push_back(labelId);
        // SendImg.push_back(bbox.x);
        // SendImg.push_back(bbox.y);
        // SendImg.push_back(bbox.w);
        // SendImg.push_back(bbox.h);

        SendDataRaw.Box += std::to_string(labelId) + "," + std::to_string(bbox.x) + "," + std::to_string(bbox.y) + "," + std::to_string(bbox.w) + "," + std::to_string(bbox.h) + ",";
        // std::cout << "画像+タイムスタンプ+物標数+box " << SendImg.size() << std::endl;

        det_num++;
    }
    // ofs.close();
    // std::cout << "画像+タイムスタンプ+物標数+box " << SendImg.size() << std::endl;

    return SendDataRaw;
}

void ObjectDetection::ImgSend(Send_Data_Raw &SendDataRaw)
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    cv::Mat mat = SendDataRaw.Img;
    int width = mat.cols;
    int height = mat.rows;
    int channel = mat.channels();
    int size = width * height * channel;
    std::string data_str(reinterpret_cast<char const *>(mat.data), size);

    // std::cout << "width : " << width << std::endl;
    // std::cout << "height : " << height << std::endl;
    // std::cout << "channel : " << channel << std::endl;
    // std::cout << "data_str : " << data_str << std::endl;
    // std::cout << "SendDataRaw.TimeStamp : " << SendDataRaw.TimeStamp << std::endl;
    // std::cout << "SendDataRaw.Count : " << SendDataRaw.Count << std::endl;
    // std::cout << "SendDataRaw.Box : " << SendDataRaw.Box << std::endl;
    // std::cout << std::endl << std::endl;

    SendData::Data DT;
    DT.set_width(width);
    DT.set_height(height);
    DT.set_channel(channel);
    DT.set_data(data_str);
    DT.set_timestamp(SendDataRaw.TimeStamp);
    DT.set_count(SendDataRaw.Count);
    DT.set_boxdata(SendDataRaw.Box);

    // std::cout << "DT.width : " << DT.width << std::endl;
    // std::cout << "DT.height : " << DT.height << std::endl;
    // std::cout << "DT.channel : " << DT.channel << std::endl;
    // std::cout << "DT.data : " << DT.data << std::endl;
    // std::cout << "DT.timestamp : " << DT.timestamp << std::endl;
    // std::cout << "DT.count : " << DT.count << std::endl;
    // std::cout << "DT.boxdata : " << DT.boxdata << std::endl;
    // std::cout << std::endl << std::endl;
    size_t Bsize = DT.ByteSize();
    std::cout << "Bsize : " << Bsize << std::endl;
    std::vector<unsigned char> SendImg(Bsize);
    DT.SerializeToArray(&SendImg[0],Bsize);
    //std::vector<uint8_t> SendImg(Bsize);
    //size_t width_Size = width.ByteSize();
    // std::cout << "width_Size : " << width_Size << std::endl;
    // std::vector<unsigned char> TestVector(1);
    // TestVector[0] = 'h';
    // std::cout << "SendImg : " << SendImg.size() << std::endl;
    std::cout << "SendImg[0] : " << SendImg[1000] << std::endl;
    std::cout << "SendImg[0] : " << std::bitset<8>(SendImg[1000]) << std::endl;
    // std::cout << "SendImg[1] : " << SendImg[1] << std::endl;
    // std::cout << "SendImg[1] : " << std::bitset<8>(SendImg[1]) << std::endl;
    // std::cout << "SendImg[1] : " << std::bitset<8>(SendImg[1]) << std::endl;
    // std::cout << "SendImg[2] : " << std::bitset<8>(SendImg[2]) << std::endl;
    // std::cout << "SendImg[3] : " << std::bitset<8>(SendImg[3]) << std::endl;
    // std::cout << "SendImg[4] : " << std::bitset<8>(SendImg[4]) << std::endl;

    /* IP アドレス、ポート番号、ソケット */
    char destination[80];
    unsigned short port = 50013; //test環境 個人PC
    int dstSocket;

    /* sockaddr_in 構造体 */
    struct sockaddr_in dstAddr;

    /* 各種パラメータ */
    int status;
    int numsnt;

    /************************************************************/
    /* 相手先アドレスの入力 */
    // destination = ("172.23.79.139");
    //    printf("Connect to ? : (name or IP address) ");
    //    scanf("%s", destination);

    /* sockaddr_in 構造体のセット */
    memset(&dstAddr, 0, sizeof(dstAddr));
    dstAddr.sin_port = htons(port);
    dstAddr.sin_family = AF_INET;
    dstAddr.sin_addr.s_addr = inet_addr("172.23.126.159"); // test環境 個人PCへ

    /* ソケット生成 */
    dstSocket = socket(AF_INET, SOCK_STREAM, 0);

    /* 接続 */
    // printf("Trying to connect to %s: \n", destination);
    connect(dstSocket, (struct sockaddr *)&dstAddr, sizeof(dstAddr));
    // std::cout << senddata.TimeStamp << std::endl;
    // std::cout << senddata.Count << std::endl;
    // std::cout << senddata.Box << std::endl;

    /* パケット送出 */
    // send(dstSocket, toSendText, 5, 0);
    // send(dstSocket, toSendText, strlen(toSendText)+1, 0);
    send(dstSocket, (const char *)(&SendImg[0]), SendImg.size(), 0);
    // send(dstSocket, SendImg.data(), SendImg.size(), 0);
    // std::cout << SendImg.size() << std::endl;

    /* ソケット終了 */
    close(dstSocket);
}

void ObjectDetection::Draw(cv::Mat &mat)
{

    cv::Mat matToServer = mat.clone(); //アノテーション用にオリジナル画像をコピー
    // matToServer = mat.clone();

    auto &track_list = tracker_.GetTrackList();
    std::vector<PositionStatus::ToSendInfo> to_send_vec; // vector to send message (not related with calculation)

    /* Draw on NormalView */
    cv::rectangle(mat, roi_, CommonHelper::CreateCvColor(0, 0, 0), 2);
    int32_t det_num = 0;

    for (auto &track : track_list)
    {
        if (track.GetUndetectedCount() > 0)
            continue;

        const auto &bbox = track.GetLatestData().bbox_raw;
        cv::rectangle(mat, cv::Rect(bbox.x, bbox.y, bbox.w, bbox.h), CommonHelper::CreateCvColor(0, 0, 0), 1);

        det_num++;
    }
    CommonHelper::DrawText(mat, "DET: " + std::to_string(det_num) + ", TRACK: " + std::to_string(tracker_.GetTrackList().size()), cv::Point(0, 20), 0.7, 2, CommonHelper::CreateCvColor(0, 0, 0), CommonHelper::CreateCvColor(220, 220, 220));
    const auto &pointlist = coordinate_.GetPointList();
    /*write area*/
    for (auto &points : pointlist)
    {
        cv::polylines(mat, points, true, CommonHelper::CreateCvColor(255, 0, 0), 3, 4);
    }

    for (auto &track : track_list)
    {
        if (track.GetDetectedCount() < 2)
            continue;

        auto &bbox = track.GetLatestData().bbox;
        bbox.time = GetFrameTime();
        const auto &id = track.GetId();

        /* Use white rectangle for the object which was not detected but just predicted */
        cv::Scalar color = bbox.score == 0 ? CommonHelper::CreateCvColor(255, 255, 255) : GetColorForId(track.GetId());
        cv::rectangle(mat, cv::Rect(bbox.x, bbox.y, bbox.w, bbox.h), color, 2);

        auto positionlist = coordinate_.GetCoordinateList(); // positions which define area //エリアを定義する点群
        // object is in area or not オブジェクトがエリアに入っているかどうか
        for (auto &positions : positionlist)
        {
            if (position_status_.PositionCheck(bbox, positions))
            {
                /*calculation*/
                /*latitude, longitude*/
                auto now_position = position_status_.PositionConversion(bbox, positions); // now_position = {latitude, longitude}

                // velocity, accelerate, azimuth, label, class_id, label, frame_counter
                auto speeds = position_status_.SpeedCalculation2(track); // speeds = {speed, accelerate, azimuth, label}

                /*identification*/
                auto identification = position_status_.SetIdentificationParam(bbox.label, id, track.GetDetectedCount());

                /*time*/
                auto tmp_time = GetSendTime();
                auto times = position_status_.TimeConversion(tmp_time); // set time to send
                CommonHelper::DrawText(mat, "Lat :" + std::to_string(now_position.latitude) + "Lon :" + std::to_string(now_position.longitude), cv::Point(bbox.x, bbox.y), 0.6, 2, CommonHelper::CreateCvColor(0, 0, 0), CommonHelper::CreateCvColor(220, 220, 220));

                // put together information to send message //情報をまとめてるだけ
                auto tmp_allinfo = position_status_.ToAllInfo(now_position, speeds, identification, times, bbox);
                to_send_vec.push_back(tmp_allinfo);
            }
        }
        // return new vector
        //  auto basic_message_list = basic_message_.get_status(to_send_vec, class_id_map);
        //  send_message_.messageSend(basic_message_list);

        // send_message

        // auto& track_history = track.GetDataHistory();
        // for (int32_t i = 1; i < static_cast<int32_t>(track_history.size()); i++) {
        //     cv::Point p0(track_history[i].bbox.x + track_history[i].bbox.w / 2, track_history[i].bbox.y + track_history[i].bbox.h);
        //     cv::Point p1(track_history[i - 1].bbox.x + track_history[i - 1].bbox.w / 2, track_history[i - 1].bbox.y + track_history[i - 1].bbox.h);
        //     cv::line(mat, p0, p1, GetColorForId(track.GetId()));
        // }
    }

    //アノテーション用データ送信
    auto SendDataRaw = SendImgCreate(matToServer);
    ImgSend(SendDataRaw);

    auto basic_message_list = basic_message_.get_status(to_send_vec, class_id_map);
    send_message_.messageSend(basic_message_list);
}

cv::Scalar ObjectDetection::GetColorForId(int32_t id)
{
    static constexpr int32_t kMaxNum = 100;
    static std::vector<cv::Scalar> color_list;
    if (color_list.empty())
    {
        std::srand(123);
        for (int32_t i = 0; i < kMaxNum; i++)
        {
            color_list.push_back(CommonHelper::CreateCvColor(std::rand() % 255, std::rand() % 255, std::rand() % 255));
        }
    }
    return color_list[id % kMaxNum];
}