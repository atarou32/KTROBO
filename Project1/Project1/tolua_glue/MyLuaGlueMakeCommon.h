#ifndef MYLUAGLUEMAKECOMMON_H
#define MYLUAGLUEMAKECOMMON_H
#pragma once
#include "stdio.h"
#include "string.h"



#define MAKEGLUEINPUT_FILEPATH "FILEPATH"
#define MAKEGLUEINPUT_INTERFACE "INTERFACE"
#define MAKEGLUEINPUT_COLLECTION "COLLECTION"
#define MAKEGLUEINPUT_CONSTRUCTOR "CONSTRUCTOR"
#define MAKEGLUEINPUT_DESTRUCTOR "DESTRUCTOR"
#define MAKEGLUEINPUT_COLLECTED "COLLECTED"








#define TEMPHAIRETU_SIZE 32
#define TEMPSTRING_SIZE 1024
#define HIKISUU_MAX 16
#define IN_
#define OUT_
#define INOUT_
#define TO_LUA
#define COLLECTED
#define YARITORI

// コレクションクラスの名前はコレクションされるクラスにsをつけた名前にすること
// TO_LUA virtual INOUT COLLECTED YARITORI  型名 の順に書くこと
// COLLECTED を使用する際には親子関係を登録するなどの場合に別の親コレクトクラスインデックスを設定しないようにLua側で気をつけること
// collectionクラスには　collectされるインターフェースを返すメソッド IMesh* getInterface(int index); が実装されていること
// collectionクラスには　collectされるインスタンスを返すメソッド　Mesh* getInstance(int index); が定義されていること
// void の場合 lua側の関数の返り値は引数と同じ数が返る
// 何かを返す場合は 先頭か後ろに本来の返り値が返る
// IN_ をつけていればそのまま返る
// OUT_ をつけていれば更新されて返る lua側で配列のサイズを確保すること
// INOUT_ をつけていれば更新されて返る
// COLLECTED をつけていればcollectionクラスのインデックス、collectされるクラスのインデックスが引数となる
// YARITORI は現状ｃ＋＋側のクラスとlua側のクラスを用意できていないので
//          それの掛け渡しとなる関数を自分で書くことにする(lua側では連想配列をクラスの表現として使おうと思う)
//          クラスのポインタを扱う場合でCOLLECTEDでない場合はYARITORI をつける必要がある
// COLLECTED がついている場合　lua側での引数はテーブルとなり、collection_index class_index をキーにして値はインデックスを持つ 
//                             collection_name, class_nameももつ
// クラスポインタが引数となる場合 複数の要素を持つ配列ではなくひとつだけと考えて処理を行う。
// float とか　int のポインタが引き数となる場合　複数の要素を持つ配列として処理を行う。TEMPHAIRETU_SIZE　を使用する。
// コレクションされるクラスのコレクションなどの定義はできない
// インターフェースを返り値としたり引数としたらだめ　コレクトされるクラスを引数にする
// ｃ＋＋側でのnull をクラス引数に指定したい場合は空の連想配列をlua側から指定すること
// 例　makeInstanced(Mesh* mesh, Mesh* skeleton, MeshInstanced* parent_mesh); のようなものがありparent_meshをNULLにしたい場合は
// Lua側で　MeshInstanceds_makeInstanced(collection_index,collected_index,  mesh, skeleton, {}); のように指定する
// Lua側では引数の最初にcollection_index を指定することが必要になる→Lua側でのクラス定義を考えることでこれも隠蔽する予定
// collectionクラスはcollectされるクラスのコンストラクタを持つ
// 例 int makeInstanced(Mesh* mesh, Mesh* skeleton, MeshInstanced* parent_mesh);
// MeshInstanceds_makeInstanced() 関数（LUA側）はintではなく　クラス連想配列を返すようにする 
// クラスポインタが引数の場合OUT_ を指定しても反映されない YARITORIの場合は反映される？
// 返り値はポインタを扱うのはクラスのみ　COLLECTED　か　YARITORI を指定する必要がある
// COLLECTED が返り値の関数は登録できない
// YARITORI のコンストラクタはvoidが引数のものが存在していること // 例：MYVECTOR4 pos;

#define COLLECTION_INDEX_LUA_FIELD "collection_index"
#define COLLECTION_CLASS_LUA_FIELD "collection_class_name"
#define COLLECTED_INDEX_LUA_FIELD "collected_index"
#define COLLECTED_CLASS_LUA_FIELD "collected_class_name"
#define COLLECTED_INTERFACE_LUA_FIELD "collected_interface_name"

int strpos(char *haystack, char *needle)
{

   if (strlen(haystack) ==0) return -1;

   char *p = strstr(haystack, needle);
   if (p)
      return p - haystack;
   return -1;   // Not found = -1.
}

void mystrcpy(char* dst, int max, int pos, char* src) {
	int count = 0;
	int mypos = pos;
	while (count < max && (src[mypos] != '\0')) {
		dst[count] = src[mypos];
		mypos++;
		count++;
	}
	dst[max-1] = '\0';
}

void mystrcpy2(char *dst, int max, int mpos,  char* src) {
	int count = 0;
	int mypos = mpos;
	while (count < max && mypos > count && (src[count] != '\0')) {
		dst[count] = src[count];
		count++;
	}
	dst[max-1] = '\0';

}





#endif

