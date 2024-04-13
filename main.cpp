#include <Novice.h>
#include <cmath>
#include <assert.h>
#include "Vector3.h"

using namespace std;

//間隔
static const int kRowHeight = 20;
static const int kColumnWidth = 60;

struct Matrix4x4
{
	float m[4][4];
};

//行列の積
static Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2)
{
	Matrix4x4 result{};
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				result.m[i][j] += m1.m[i][k] * m2.m[k][j];
			}
		}
	}
	return result;
}


//拡大縮小行列
static Matrix4x4 MakeScaleMatrix(const Vector3& scale)
{
	Matrix4x4 result{};
	result.m[0][0] = scale.x;
	result.m[1][1] = scale.y;
	result.m[2][2] = scale.z;
	result.m[3][3] = 1.0f;
	return result;
}

//X軸の回転行列
static Matrix4x4 MakeRotateXMatrix(float radian)
{
	Matrix4x4 result{};
	result.m[0][0] = 1.0f;
	result.m[1][1] = cos(radian);
	result.m[1][2] = sin(radian);
	result.m[2][1] = -sin(radian);
	result.m[2][2] = cos(radian);
	result.m[3][3] = 1.0f;
	return result;
}

//Y軸の回転行列
static Matrix4x4 MakeRotateYMatrix(float radian)
{
	Matrix4x4 result{};
	result.m[0][0] = cos(radian);
	result.m[0][2] = -sin(radian);
	result.m[1][1] = 1.0f;
	result.m[2][0] = sin(radian);
	result.m[2][2] = cos(radian);
	result.m[3][3] = 1.0f;
	return result;
}

//Z軸の回転行列
static Matrix4x4 MakeRotateZMatrix(float radian)
{
	Matrix4x4 result{};
	result.m[0][0] = cos(radian);
	result.m[0][1] = sin(radian);
	result.m[1][0] = -sin(radian);
	result.m[1][1] = cos(radian);
	result.m[2][2] = 1.0f;
	result.m[3][3] = 1.0f;
	return result;
}

//平行移動行列
static Matrix4x4 MakeTranslateMatrix(const Vector3& translate)
{
	Matrix4x4 result{};
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (i == j)
			{
				result.m[i][j] = 1.0f;
			}
			else
			{
				result.m[i][j] = 0.0f;
			}
		}
	}
	result.m[3][0] = translate.x;
	result.m[3][1] = translate.y;
	result.m[3][2] = translate.z;
	return result;
}

//三次元アフィン変換行列
static Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& radian, const Vector3& translate)
{
	return  Multiply(MakeScaleMatrix(scale), Multiply(Multiply(MakeRotateXMatrix(radian.x), Multiply(MakeRotateYMatrix(radian.y), MakeRotateZMatrix(radian.z))), MakeTranslateMatrix(translate)));
}

// 行列のコメント
static void MatrixScreenPrint(int x, int y, Matrix4x4 matrix, const char* label)
{
	for (int row = 0; row < 4; row++)
	{
		for (int colum = 0; colum < 4; colum++)
		{
			Novice::ScreenPrintf(x + colum * kColumnWidth, y + row * kRowHeight + 20, "%6.02f", matrix.m[row][colum]);
		}
	}
	Novice::ScreenPrintf(x, y, "%s", label);
}

const char kWindowTitle[] = "LE2B_09_キクチ_ケンタ_提出用課題";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	Vector3 scale = { 1.2f,0.79f,-2.1f };
	Vector3 rotate = { 0.4f,1.43f,-0.8f };
	Vector3 translate = { 2.7f,-4.15f,1.57f };


	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		//MakeAffineMatrixを呼び出す
		Matrix4x4 worldMatrix = MakeAffineMatrix(scale, rotate, translate);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		MatrixScreenPrint(0, 0, worldMatrix, "worldMatrix");

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
