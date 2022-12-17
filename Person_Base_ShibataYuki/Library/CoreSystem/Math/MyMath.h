

#ifndef __MY_MATH_H__
#define __MY_MATH_H__

#include <CoreSystem/Math/myVector.h>
//#include <DirectXTex.h>

//using namespace DirectX;

namespace MySpace
{
	namespace MyMath
	{
		// アライメント
		template <class T>
		struct alignas(16) AlgnasConvert
		{
		public:
			T value;
			T operator=(T f1) { value = f1; return value; }
		};


		struct ST_MyMath 
		{
			template <class T>
			static T Clamp(T value, T min, T max)
			{
				if (value <= min) { return min; }
				else if (value >= max) { return max; }
				return value;
			}
			static float Lerp(float from, float to, float t)
			{
				return from * (1.0f - t) + to * t;
			}

			// *@カーブの生成
			static float HermiteCurve(float x0, float x1, float v0, float v1, float t)
			{
				return ((t - 1) * (t - 1) * (2 * t + 1) * x0) + ((t * t) * (3 - 2 * t) * x1)
					+ ((1 - t)*(1 - t) * t * v0) + ((t - 1) * (t * t) * v1);
			}

			// *@カーブの生成
			static std::vector<Vector2> MakeHermiteCurve(Vector2 firePos, Vector2 vec1, Vector2 targetPos, Vector2 vec2, int max)
			{
				// 位置ベクトルx0
				float x0 = firePos.x;
				float y0 = firePos.y;

				// 位置ベクトルx1
				float x1 = targetPos.x;
				float y1 = targetPos.y;

				// 接線ベクトルv0
				float v0x = (x0 - vec1.x) - x0;
				float v0y = (y0 - vec1.y) - y0;

				// 接線ベクトルv1
				float v1x = x1 - (x1 - vec2.x);
				float v1y = y1 - (y1 - vec2.y);

				float t = 0;
				std::vector<Vector2> ret;
				for (int cnt = 0; cnt < max; ++cnt)
				{
					Vector2 point;
					t = (float)cnt / max;
					point.x = HermiteCurve(x0, x1, v0x, v1x, t);
					point.y = HermiteCurve(y0, y1, v0y, v1y, t);

					ret.push_back(point);
				}
				return ret;
			}

			static std::vector<Vector3> MakeHermiteCurve(Vector3 firePos, Vector3 vec1, Vector3 targetPos, Vector3 vec2, int max)
			{
				// 位置ベクトルx0
				auto pos0 = firePos;

				// 位置ベクトルx1
				auto pos1 = targetPos;

				// 接線ベクトルv0
				float v0x = (pos0.x - vec1.x) - pos0.x;
				float v0y = (pos0.y - vec1.y) - pos0.y;
				float v0z = (pos0.z - vec1.z) - pos0.z;

				// 接線ベクトルv1
				float v1x = pos1.x - (pos1.x - vec2.x);
				float v1y = pos1.y - (pos1.y - vec2.y);
				float v1z = pos1.z - (pos1.z - vec2.z);

				float t = 0;
				std::vector<Vector3> ret;
				for (int cnt = 0; cnt < max; ++cnt)
				{
					Vector3 point;
					t = (float)cnt / max;
					point.x = HermiteCurve(pos0.x, pos1.x, v0x, v1x, t);
					point.y = HermiteCurve(pos0.y, pos1.y, v0y, v1y, t);
					point.z = HermiteCurve(pos0.z, pos1.z, v0z, v1z, t);

					ret.push_back(point);
				}
				return ret;
			}

			static float MathMove(float pos, float vel, float acc, float time)
			{
				return (pos + vel * time + (1.0f / 2 * (acc * time * time)));
			}

			// 行列からクオータニオンを生成する
			//void DX11GetQtfromMatrix(const XMFLOAT4X4& mat, XMFLOAT4& qt) {

			//	XMMATRIX mtx;
			//	XMVECTOR outvec;

			//	mtx = XMLoadFloat4x4(&mat);

			//	outvec = XMQuaternionRotationMatrix(mtx);

			//	XMStoreFloat4(&qt, outvec);
			//}

			////指定軸を中心に回転したクオータニオンを生成する
			//void DX11QtRotationAxis(XMFLOAT4& qt, XMFLOAT4& axis, float angle) 
			//{
			//	XMVECTOR outqt;
			//	XMVECTOR axisvec;

			//	axisvec = XMLoadFloat4(&axis);

			//	outqt = XMQuaternionRotationAxis(axisvec, (angle*XM_PI) / 180.0f);

			//	XMStoreFloat4(&qt, outqt);
			//}

			//
			//// クオータニオンの掛け算
			//void DX11QtMul(XMFLOAT4& outqt, const XMFLOAT4& qt1, const XMFLOAT4& qt2) {
			//	XMVECTOR qtvec1;
			//	XMVECTOR qtvec2;
			//	XMVECTOR outqtvec;

			//	qtvec1 = XMLoadFloat4(&qt1);
			//	qtvec2 = XMLoadFloat4(&qt2);

			//	outqtvec = XMQuaternionMultiply(qtvec1, qtvec2);

			//	XMStoreFloat4(&outqt, outqtvec);
			//}

			//Vector3	right() {
			//	DirectX::XMFLOAT4X4 mat;
			//	DX11MtxFromQt(mat, qt);

			//	return Vector3{ mat._11,mat._12,mat._13 };
			//}
			//Vector3 up() {
			//	DirectX::XMFLOAT4X4 mat;
			//	DX11MtxFromQt(mat, qt);

			//	return Vector3{ mat._21,mat._22,mat._23 };
			//}
			//Vector3	forward() {
			//	DirectX::XMFLOAT4X4 mat;
			//	DX11MtxFromQt(mat, qt);

			//	return Vector3{ mat._31,mat._32,mat._33 };
			//}

			//void RotateXaxis(float angle) {
			//	DirectX::XMFLOAT4 q, axis;
			//	axis = { 1,0,0,0 };

			//	DX11QtRotationAxis(q, axis, angle);

			//	DX11QtMul(qt, qt, q);
			//}
			//void RotateYaxis(float angle) {
			//	DirectX::XMFLOAT4 q, axis;
			//	axis = { 0,1,0,0 };

			//	DX11QtRotationAxis(q, axis, angle);

			//	DX11QtMul(qt, qt, q);
			//}
			//void RotateZaxis(float angle) {
			//	DirectX::XMFLOAT4 q, axis;
			//	axis = { 0,0,1,0 };

			//	DX11QtRotationAxis(q, axis, angle);

			//	DX11QtMul(qt, qt, q);
			//}

			//void RotateLocalXaxis(float angle) {
			//	DirectX::XMFLOAT4 q, axis;
			//	axis = right();

			//	DX11QtRotationAxis(q, axis, angle);

			//	DX11QtMul(qt, qt, q);
			//}
			//void RotateLocalYaxis(float angle) {
			//	DirectX::XMFLOAT4 q, axis;
			//	axis = up();

			//	DX11QtRotationAxis(q, axis, angle);

			//	DX11QtMul(qt, qt, q);
			//}
			//void RotateLocalZaxis(float angle) {
			//	DirectX::XMFLOAT4 q, axis;
			//	axis = forward();

			//	DX11QtRotationAxis(q, axis, angle);

			//	DX11QtMul(qt, qt, q);
			//}

			//auto GetLookQt(Vector3 lookPos, Vector3 _up) {
			//	DirectX::XMFLOAT4 _qt;
			//	Vector3 z = (lookPos - position).Normalize();
			//	Vector3 x = _up.Cross(z).Normalize();
			//	Vector3 y = z.Cross(x).Normalize();

			//	DirectX::XMFLOAT4X4 mat;
			//	mat._11 = x.x; mat._12 = x.y; mat._13 = x.z;
			//	mat._21 = y.x; mat._22 = y.y; mat._23 = y.z;
			//	mat._31 = z.x; mat._32 = z.y; mat._33 = z.z;

			//	DX11GetQtfromMatrix(mat, _qt);
			//	return _qt;
			//}
			//void SlerpRotate(DirectX::XMFLOAT4 toqt, const float value) {
			//	DX11QtSlerp(qt, toqt, value, qt);
			//}
			//void LookAt(Vector3 lookPos, Vector3 _up) {
			//	qt = GetLookQt(lookPos, _up);
			//}
			//void LookSlerp(Vector3 lookPos, Vector3 _up, const float value) {

			//	DirectX::XMFLOAT4 _qt;
			//	_qt = GetLookQt(lookPos, _up);
			//	DX11QtSlerp(qt, _qt, value, qt);
			//}

			//auto GetLookStreat(Vector3 lookPos, Vector3 _up) {
			//	DirectX::XMFLOAT4 _qt;
			//	Vector3 z = (lookPos - position).Normalize();
			//	Vector3 x = _up.Cross(z).Normalize();
			//	Vector3 y = z.Cross(x).Normalize();

			//	DirectX::XMFLOAT4X4 mat;
			//	mat._11 = x.x; mat._12 = x.y; mat._13 = x.z;
			//	mat._21 = y.x; mat._22 = y.y; mat._23 = y.z;
			//	mat._31 = z.x; mat._32 = z.y; mat._33 = z.z;

			//	DX11GetQtfromMatrix(mat, _qt);
			//	return _qt;
			//}

		};

		
	}
}


#endif // !__MY_MATH_H__
