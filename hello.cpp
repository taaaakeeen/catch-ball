#include <stdio.h>
#include <unistd.h>

// 戻り値あり
// 型名 関数名(型名 引数1, 型名 引数2, ・・・) {
//     処理
//     return オブジェクト
// }

// 戻り値なし
// void 関数名(型名 引数1, 型名 引数2, ・・・) {
//     処理
// }

void piyo(){
    printf("piyo\n");
}

int main(void) {
    while(1){
        printf("hoge\n");
        sleep(3);
        piyo();
    }
    // return 0;
}