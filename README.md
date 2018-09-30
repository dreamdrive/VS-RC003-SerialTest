# VS-RC003-SerialTest

* Vstone社 のロボット用マイコンボード VS-RC003のメモリマップをPCから読み書きするプログラムです。
  * 本ソースはMicrosoft Visual C++（VC 2017）用です。それ以外の開発環境をご利用の場合は、通信処理部分等をお使いの環境に応じて書き換えてください。
  * Vstone社「VS-RC003/HVによるシリアル通信プログラムサンプル	'SerialSample.cpp'」をベースにしています。
  * 'TARGET_PORT'で定義されたシリアルポート番号を、お使いのPCの環境に応じて書き換えてください。
  * 本家の通信のバイトオーダーはLEですが、関数の引数／戻り値はBEになっています。

Dream Drive !! みっちー http://dream-drive.net  

* Vstone社「VS-RC003/HVによるシリアル通信プログラムサンプル	'SerialSample.cpp'」をベースにしています。
  * VS-RC003HVシリアル通信資料 (https://www.vstone.co.jp/products/vs_rc003hv/download/VSRC003_serial_manual.pdf)
  * S-RC003HVシリアル通信 C言語サンプルソース (https://www.vstone.co.jp/products/vs_rc003hv/download/SerialSample.cpp)
* VS-RC003とは (https://www.vstone.co.jp/products/vs_rc003hv/index.html)
