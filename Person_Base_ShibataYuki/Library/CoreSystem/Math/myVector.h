


#ifndef __MY_VECTOR_H__
#define __MY_VECTOR_H__

#include <CoreSystem/Util/cerealCommon.h>

//#include <DirectXTex.h>
#include <DirectXMath.h>
#include <math.h>

using namespace DirectX;

namespace MySpace
{
	namespace MyMath
	{

		// 構造体XMFLOAT型を継承したAdapterパターン
		class Vector2 : public XMFLOAT2
		{
		private:
			// シリアライズ
			friend class cereal::access;
			template<class Archive>
			void save(Archive & archive) const
			{
				archive(CEREAL_NVP(x), CEREAL_NVP(y));
			}
			template<class Archive>
			void load(Archive & archive)
			{
				archive(CEREAL_NVP(x), CEREAL_NVP(y));
			}
		public:
			/*	float x;
				float y;*/
			Vector2() { x = y = 0; }
			Vector2(float x1, float y1) { x = x1; y = y1; }
			Vector2(XMFLOAT2 f) { x = f.x; y = f.y; }

			// 演算子のオーバーロード
			const XMFLOAT2 operator= (const Vector2& vec)  { x = vec.x; y = vec.y;   return *this; }
			const XMFLOAT2 operator+= (const Vector2& vec) { x += vec.x; y += vec.y; return *this; }
			const Vector2 operator-= (const XMFLOAT2& vec) { x -= vec.x; y -= vec.y; return *this; }
			const Vector2 operator*= (const XMFLOAT2& vec) { x *= vec.x; y *= vec.y; return *this; }
			const Vector2 operator/= (const XMFLOAT2& vec) { x /= vec.x; y /= vec.y; return *this; }
			const Vector2 operator*= (const float f) { x *= f; y *= f; return *this; }
			const Vector2 operator/= (const float f) { x /= f; y /= f; return *this; }
			Vector2 operator+(const XMFLOAT2& vec) { return Vector2(x + vec.x, y + vec.y); }
			Vector2 operator-(const XMFLOAT2& vec) { return Vector2(x - vec.x, y - vec.y); }
			Vector2 operator*(const XMFLOAT2& vec) { return Vector2(x * vec.x, y * vec.y); }
			Vector2 operator/(const XMFLOAT2& vec) { return Vector2(x / vec.x, y / vec.y); }
			Vector2 operator* (const float & f) const { return Vector2(x * f, y * f); }
			Vector2 operator/ (const float & f) const { return Vector2(x / f, y / f); }
			bool operator== (const Vector2& vec)const { return (x == vec.x) && (y == vec.y); }
			bool operator!= (const Vector2& vec)const { return !(*this == vec); }
			Vector2 operator-() const { return Vector2(-x, -y); }
			Vector2 operator+() const { return Vector2(x, y); }
			operator float* () const { return (float *)&x; }
			operator const float* () const { return (const float *)&x; }


			float Length()const { return sqrtf(x * x + y * y); }
			Vector2 Normalize()
			{
				float length = Length();
				if (length == 1.0f)return *this;
				if (length == 0.0f)return Vector2(0, 0);
				*this /= length;
				return *this;
			}

			// 静的メンバ関数
			static Vector2 Lerp(const Vector2& from, const Vector2& to, float value) {
				value = 0;
				return from * (1.0f - value) + to * value;
			}

			// 正規化した値を返す
			static Vector2 Normalize(const Vector2 & vec2)
			{
				float value = vec2.Length();
				if (value == 1.0f) return vec2; // 既に正規化済みなら何もしない
				if (value == 0.0f) return Vector2(0, 0);
				return (vec2 / value);
			}

			static float Dot(const Vector2 & vec1, const Vector2 & vec2) { return (vec1.x * vec2.x + vec1.y * vec2.y); }
			static float Cross(const Vector2 & vec1, const Vector2 & vec2) { return (vec1.x * vec2.y - vec1.y * vec2.x); }

			template<class T>
			T Convert()
			{
				T ret = { x , y };
				return ret;
			}
		};


		class Vector3 : public XMFLOAT3
		{
		private:
			// シリアライズ
			friend class cereal::access;
			template<class Archive>
			void save(Archive & archive) const
			{
				archive(CEREAL_NVP(x), CEREAL_NVP(y), CEREAL_NVP(z));
			}
			template<class Archive>
			void load(Archive & archive)
			{
				archive(CEREAL_NVP(x), CEREAL_NVP(y), CEREAL_NVP(z));
			}
		public:
			// コンストラクタ
			Vector3() { x = 0; y = 0; z = 0; };
			// *@xyz
			Vector3(float x1, float y1, float z1) { x = x1; y = y1; z = z1; }
			Vector3(XMFLOAT3 vel) { x = vel.x; y = vel.y; z = vel.z; }

			//--- 演算子のオーバーロード
			XMFLOAT3 operator= (const Vector3 vec) { (x = vec.x, y = vec.y, z = vec.z); return *this; }
			XMFLOAT3 operator+= (const Vector3 vec) { (x += vec.x, y += vec.y, z += vec.z); return *this; }
			XMFLOAT3 operator-= (const Vector3 vec) { (x -= vec.x, y -= vec.y, z -= vec.z); return *this; }
			XMFLOAT3 operator*= (const Vector3 vec) { (x *= vec.x, y *= vec.y, z *= vec.z); return *this; }
			Vector3 operator+ (const XMFLOAT3 vec) { return Vector3(x + vec.x, y + vec.y, z + vec.z); }
			Vector3 operator- (const XMFLOAT3 vec) { return Vector3(x - vec.x, y - vec.y, z - vec.z); }
			Vector3 operator* (const XMFLOAT3 vec) { return Vector3(x * vec.x, y * vec.y, z * vec.z); }
			Vector3 operator/ (const XMFLOAT3 vec) { return Vector3(x / vec.x, y / vec.y, z / vec.z); }
			void operator+= (const XMFLOAT3 vec) { x += vec.x; y += vec.y; z += vec.z; }
			void operator-= (const XMFLOAT3 vec) { x -= vec.x; y -= vec.y; z -= vec.z; }
			void operator*= (const XMFLOAT3 vec) { x *= vec.x; y *= vec.y; z *= vec.z; }
			void operator/= (const XMFLOAT3 vec) { x /= vec.x; y /= vec.y; z /= vec.z; }

			void operator*= (const float f) { x *= f; y *= f; z *= f; }
			void operator/= (const float f) { x /= f; y /= f; z /= f; }
			Vector3 operator+(const Vector3& vec) { return Vector3(x + vec.x, y + vec.y, z + vec.z); }
			Vector3 operator-(const Vector3& vec) { return Vector3(x - vec.x, y - vec.y, z - vec.z); }
			Vector3 operator*(const Vector3& vec) { return Vector3(x * vec.x, y * vec.y, z * vec.z); }
			Vector3 operator/(const Vector3& vec) { return Vector3(x / vec.x, y / vec.y, z / vec.z); }
			Vector3 operator* (const float & f) const { return Vector3(x * f, y * f ,z * f); }
			Vector3 operator/ (const float & f) const { return Vector3(x / f, y / f, z / f); }
			Vector3 operator= (const float & f) const { return Vector3(f, f, f); }
			bool operator== (const Vector3& vec)const { return (x == vec.x) && (y == vec.y) && (z == vec.z); }
			bool operator!= (const Vector3& vec)const { return !(*this == vec); }
			Vector3 operator-() const { return Vector3(-x, -y, -z); }
			Vector3 operator+() const { return Vector3(x, y, z); }
			operator float* () const { return (float *)&x; }
			operator const float* () const { return (const float *)&x; }

			Vector3 zero() { return Vector3(0, 0, 0); }
			Vector3 one() { return Vector3(1, 1, 1); }
			Vector3 forward() { return Vector3(0, 0, 1); }
			Vector3 left() { return Vector3(1, 0, 0); }
			Vector3 right() { return Vector3(-1, 0, 0); }

			bool check(Vector3 v1)
			{
				if (v1.x != x)return false;
				if (v1.y != y)return false;
				if (v1.z != z)return false;
				return true;
			}

			static Vector3 check(Vector3 v1, Vector3 v2)
			{
				Vector3 v0(0, 0, 0);
				if (v1.x != v2.x)v0.x = 1;
				if (v1.y != v2.y)v0.y = 1;
				if (v1.z != v2.z)v0.z = 1;
				return v0;
			}

			float Length()const { return sqrtf(x * x + y * y + z * z); }
			Vector3 Normalize()
			{
				float length = Length();
				if (length == 1.0f)return *this;
				if (length == 0.0f)return Vector3(0, 0, 0);
				*this /= length;
				return *this;
			}

			// 静的メンバ関数
			// 内積
			static float Dot(Vector3 vec1, Vector3 vec2)
			{
				return (vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z);
			}

			// 外積
			static Vector3 Cross(Vector3 vec1, Vector3 vec2)
			{
				return Vector3(vec1.y * vec2.z - vec1.z * vec2.y,
					vec1.z * vec2.x - vec1.x * vec2.z,
					vec1.x * vec2.y - vec1.y * vec2.x);
			}

			// ベクトルの長さ
			static float Length(Vector3 vec)
			{
				return sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
			}

			// 二点間の距離
			static float Length(Vector3 vec1, Vector3 vec2)
			{
				Vector3 vec = vec1 - vec2;
				return sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
			}

			// ベクトルの正規化
			static Vector3 Normalize(Vector3 vec)
			{
				//Vector3 vec = v;
				float len = Length(vec);
				if (!len) return vec;
				vec.x /= len;
				vec.y /= len;
				vec.z /= len;
				return vec;
			}
			//// 正規化した値を返す
			//static Vector3 Normalize(const Vector3 & vec)
			//{
			//	float value = vec.Length();
			//	if (value == 1.0f) return vec; // 既に正規化済みなら何もしない
			//	if (value == 0.0f) return Vector3(0, 0, 0);
			//	return (vec / value);
			//}

			static Vector3 Lerp(const Vector3& from, const Vector3& to, float value) {
				value = 0;
				return from * (1.0f - value) + to * value;
			}


			//static float Dot(const Vector3 & vec1, const Vector3 & vec2) { return (vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z); }
			/*static Vector3 Cross(const Vector3 & vec1, const Vector3 & vec2) {
				return Vector3(vec1.y * vec2.z - vec1.z * vec2.y,
					vec1.z * vec2.x - vec1.x * vec2.z, vec1.x * vec2.y - vec1.y * vec2.x);
			}*/

			// *@一番大きいメンバ関数を返す
			float GetLargeValue()
			{
				if (x < y && z < y)
						return y;
				if (y < x && z < x)
						return x;
				//if (x < z && y < z)
				return z;
			}

			template<class T>
			T Convert()
			{
				T ret = { x , y, z };
				return ret;
			}
		};

		class Vector4 : public XMFLOAT4
		{
		private:
			// シリアライズ
			friend class cereal::access;
			template<class Archive>
			void save(Archive & archive) const
			{
				archive(CEREAL_NVP(x), CEREAL_NVP(y), CEREAL_NVP(z), CEREAL_NVP(w));
			}
			template<class Archive>
			void load(Archive & archive)
			{
				archive(CEREAL_NVP(x), CEREAL_NVP(y), CEREAL_NVP(z), CEREAL_NVP(w));
			}
		public:
			Vector4() { x = y = z = w = 0; };
			Vector4(float x1, float y1, float z1, float w1) { x = x1; y = y1; z = z1; w = w1; };
			Vector4(float x1, float y1, float z1) { x = x1; y = y1; z = z1; w = 1; };
			~Vector4() {};

			XMFLOAT4 operator= (const Vector4 vec) { return XMFLOAT4(x = vec.x, y = vec.y, z = vec.z, w = vec.w); }
			XMFLOAT4 operator+= (const Vector4 vec) { return XMFLOAT4(x += vec.x, y += vec.y, z += vec.z, w += vec.w); }
			Vector4 operator+ (const XMFLOAT4 vec) { return Vector4(x += vec.x, y += vec.y, z += vec.z, w += vec.w); }
			Vector4 operator- (const XMFLOAT4 vec) { return Vector4(x -= vec.x, y -= vec.y, z -= vec.z, w -= vec.w); }
			Vector4 operator* (const XMFLOAT4 vec) { return Vector4(x *= vec.x, y *= vec.y, z *= vec.z, w *= vec.w); }
			Vector4 operator/ (const XMFLOAT4 vec) { return Vector4(x /= vec.x, y /= vec.y, z /= vec.z, w /= vec.w); }
			void operator+= (const XMFLOAT4 vec) { x += vec.x; y += vec.y; z += vec.z; w += vec.w; }
			void operator-= (const XMFLOAT4 vec) { x -= vec.x; y -= vec.y; z -= vec.z; w -= vec.w; }
			void operator*= (const XMFLOAT4 vec) { x *= vec.x; y *= vec.y; z *= vec.z; w *= vec.w; }
			void operator/= (const XMFLOAT4 vec) { x /= vec.x; y /= vec.y; z /= vec.z; w /= vec.w; }

			void operator*= (const float f) { x *= f; y *= f; z *= f; w *= f; }
			void operator/= (const float f) { x /= f; y /= f; z /= f; w /= f; }
			Vector4 operator+(const Vector4& vec) { return Vector4(x + vec.x, y + vec.y, z + vec.z, w + vec.w); }
			Vector4 operator-(const Vector4& vec) { return Vector4(x - vec.x, y - vec.y, z - vec.z, w - vec.w); }
			Vector4 operator*(const Vector4& vec) { return Vector4(x * vec.x, y * vec.y, z * vec.z, w * vec.w); }
			Vector4 operator/(const Vector4& vec) { return Vector4(x / vec.x, y / vec.y, z / vec.z, w / vec.w); }
			Vector4 operator* (const float & f) const { return Vector4(x, y, z, w) * f; }
			Vector4 operator/ (const float & f) const { return Vector4(x / f, y / f, z / f, w / f); }
			bool operator== (const Vector4& vec)const { return (x == vec.x) && (y == vec.y) && (z == vec.z) && (w == vec.w); }
			bool operator!= (const Vector4& vec)const { return !(*this == vec); }
			Vector4 operator-() const { return Vector4(-x, -y, -z, -w); }
			Vector4 operator+() const { return Vector4(x, y, z, w); }
		};

		class Matrix4x4 : public XMFLOAT4X4
		{
		private:
			//--- シリアライズ
			friend class cereal::access;
			/*template<class Archive>
			void serialize(Archive & archive, XMFLOAT4X4 & m)
			{
				archive(CEREAL_NVP(m._11), CEREAL_NVP(m._12), CEREAL_NVP(m._13), CEREAL_NVP(m._14),
					CEREAL_NVP(m._21), CEREAL_NVP(m._22), CEREAL_NVP(m._23), CEREAL_NVP(m._24),
					CEREAL_NVP(m._31), CEREAL_NVP(m._32), CEREAL_NVP(m._33), CEREAL_NVP(m._34),
					CEREAL_NVP(m._41), CEREAL_NVP(m._42), CEREAL_NVP(m._43), CEREAL_NVP(m._44));
			}*/
			template<class Archive>
			void save(Archive & archive)const
			{
				archive(CEREAL_NVP(_11), CEREAL_NVP(_12), CEREAL_NVP(_13), CEREAL_NVP(_14),
					CEREAL_NVP(_21), CEREAL_NVP(_22), CEREAL_NVP(_23), CEREAL_NVP(_24),
					CEREAL_NVP(_31), CEREAL_NVP(_32), CEREAL_NVP(_33), CEREAL_NVP(_34),
					CEREAL_NVP(_41), CEREAL_NVP(_42), CEREAL_NVP(_43), CEREAL_NVP(_44));
			}
			template<class Archive>
			void load(Archive & archive)
			{
				archive(CEREAL_NVP(_11), CEREAL_NVP(_12), CEREAL_NVP(_13), CEREAL_NVP(_14),
					CEREAL_NVP(_21), CEREAL_NVP(_22), CEREAL_NVP(_23), CEREAL_NVP(_24),
					CEREAL_NVP(_31), CEREAL_NVP(_32), CEREAL_NVP(_33), CEREAL_NVP(_34),
					CEREAL_NVP(_41), CEREAL_NVP(_42), CEREAL_NVP(_43), CEREAL_NVP(_44));
			}
		public:
			Matrix4x4() {
				_11 = 0.0f; _12 = 0.0f; _13 = 0.0f; _14 = 0.0f;
				_21 = 0.0f; _22 = 0.0f; _23 = 0.0f; _24 = 0.0f;
				_31 = 0.0f; _32 = 0.0f; _33 = 0.0f; _34 = 0.0f;
				_41 = 0.0f; _42 = 0.0f; _43 = 0.0f; _44 = 0.0f;
			};
			Matrix4x4(XMFLOAT4X4 mt) {
				_11 = mt._11; _12 = mt._12; _13 = mt._13; _14 = mt._14;
				_21 = mt._21; _22 = mt._22; _23 = mt._23; _24 = mt._24;
				_31 = mt._31; _32 = mt._32; _33 = mt._33; _34 = mt._34;
				_41 = mt._41; _42 = mt._42; _43 = mt._43; _44 = mt._44;
			};
			Matrix4x4(
				float m00, float m01, float m02, float m03,
				float m10, float m11, float m12, float m13,
				float m20, float m21, float m22, float m23,
				float m30, float m31, float m32, float m33)
				:XMFLOAT4X4(
					m00, m01, m02, m03,
					m10, m11, m12, m13,
					m20, m21, m22, m23,
					m30, m31, m32, m33) {}
			~Matrix4x4() {};

			void operator= (const DirectX::XMFLOAT4X4 mt)
			{
				_11 = mt._11; _12 = mt._12; _13 = mt._13; _14 = mt._14;
				_21 = mt._21; _22 = mt._22; _23 = mt._23; _24 = mt._24;
				_31 = mt._31; _32 = mt._32; _33 = mt._33; _34 = mt._34;
				_41 = mt._41; _42 = mt._42; _43 = mt._43; _44 = mt._44;
			}
			
			Matrix4x4 operator* (const DirectX::XMFLOAT4X4 mt)
			{
				return Matrix4x4(
					_11 * mt._11, _12 * mt._12, _13 * mt._13, _14 * mt._14,
					_21 * mt._21, _22 * mt._22, _23 * mt._23, _24 * mt._24,
					_31 * mt._31, _32 * mt._32, _33 * mt._33, _34 * mt._34,
					_41 * mt._41, _42 * mt._42, _43 * mt._43, _44 * mt._44
				);
			}
			
			void operator*= (const DirectX::XMFLOAT4X4 mt)
			{
				_11 *= mt._11; _12 *= mt._12; _13 *= mt._13; _14 *= mt._14;
				_21 *= mt._21; _22 *= mt._22; _23 *= mt._23; _24 *= mt._24;
				_31 *= mt._31; _32 *= mt._32; _33 *= mt._33; _34 *= mt._34;
				_41 *= mt._41; _42 *= mt._42; _43 *= mt._43; _44 *= mt._44;
			}

			Matrix4x4 Identity() {
				_11 = 1.0f; _12 = 0.0f; _13 = 0.0f; _14 = 0.0f;
				_21 = 0.0f; _22 = 1.0f; _23 = 0.0f; _24 = 0.0f;
				_31 = 0.0f; _32 = 0.0f; _33 = 1.0f; _34 = 0.0f;
				_41 = 0.0f; _42 = 0.0f; _43 = 0.0f; _44 = 1.0f;
				return *this;
			};

			Matrix4x4 CalcWorld(Vector3 pos, Vector3 center = Vector3(0, 0, 0))
			{
				Matrix4x4 mtx = *this;
				mtx._41 = pos.x;
				mtx._42 = pos.y;
				mtx._43 = pos.z;

				XMVECTOR vCenter = XMLoadFloat3(&center);
				XMMATRIX mWorld = XMLoadFloat4x4(&mtx);
				vCenter = XMVector3TransformCoord(vCenter, mWorld);
				mWorld = XMMatrixTranslationFromVector(vCenter);
				XMStoreFloat4x4(&mtx, mWorld);
				return mtx;
			}

			Matrix4x4 Multiply(Matrix4x4 a)
			{
				auto newMtx = XMMatrixMultiply(XMLoadFloat4x4(&*this), XMLoadFloat4x4(&a));
				Matrix4x4 ret;
				XMStoreFloat4x4(&ret, newMtx);
				return ret;
			}
		};

		class Quaternion : public XMFLOAT4
		{
		private:
			// シリアライズ
			friend class cereal::access;
			template<class Archive>
			void save(Archive & archive) const
			{
				archive(CEREAL_NVP(x), CEREAL_NVP(y), CEREAL_NVP(z), CEREAL_NVP(w));
			}
			template<class Archive>
			void load(Archive & archive)
			{
				archive(CEREAL_NVP(x), CEREAL_NVP(y), CEREAL_NVP(z), CEREAL_NVP(w));
			}
		public:
			Quaternion() {};
			Quaternion(float x, float y, float z, float w) {};
			Quaternion(float x, float y, float z) {};
			~Quaternion() {};
		};

		struct Color
		{
		private:
			// シリアライズ
			friend class cereal::access;
			template<class Archive>
			void save(Archive & archive) const
			{
				archive(CEREAL_NVP(r), CEREAL_NVP(g), CEREAL_NVP(b), CEREAL_NVP(a));
			}
			template<class Archive>
			void load(Archive & archive)
			{
				archive(CEREAL_NVP(r), CEREAL_NVP(g), CEREAL_NVP(b), CEREAL_NVP(a));
			}
		public:
			float r, g, b, a;
		public:
			Color() :r(0), g(0), b(0), a(1) {};
			Color(float x, float y, float z, float w) :r(x), g(y), b(z), a(w) {}
			Color(XMFLOAT4 color) :r(color.x), g(color.y), b(color.z), a(color.w) {}

			
			void operator=(XMFLOAT4 c) { r = c.x; g = c.y; b = c.z; a = c.w; }
			void operator=(XMFLOAT3 c) { r = c.x; g = c.y; b = c.z; }
			void operator+=(Color c) { r += c.r; g += c.g; b += c.b; a += c.a; }
			void operator-=(Color c) { r -= c.r; g -= c.g; b -= c.b; a -= c.a; }
			void operator/=(Color c) { r /= c.r; g /= c.g; b /= c.b; a /= c.a; }
			void operator*=(Color c) { r *= c.r; g *= c.g; b *= c.b; a *= c.a; }

			//--- 比較
			bool operator==(XMFLOAT3 c) { return (r == c.x && g == c.y && b == c.z); }
			bool operator==(XMFLOAT4 c) { return (r == c.x && g == c.y && b == c.z && a == c.w); }
			bool operator!=(XMFLOAT3 c) { return !(*this == c); }
			bool operator!=(XMFLOAT4 c) { return !(*this == c); }

			// *@Vector4受け渡し
			Vector4 operator()()	 { return Vector4(r, g, b, a); }

			//--- 演算
			Color operator+(Color c) { return Color(r + c.r, g + c.g, b + c.b, a + c.a); }
			Color operator-(Color c) { return Color(r - c.r, g - c.g, b - c.b, a - c.a); }
			Color operator/(Color c) { return Color(r / c.r, g / c.g, b / c.b, a / c.a); }
			Color operator*(Color c) { return Color(r * c.r, g * c.g, b * c.b, a * c.a); }

			Color operator+(float f) { return Color(r + f, g + f, b + f, a + f); }
			Color operator-(float f) { return Color(r - f, g - f, b - f, a - f); }
			Color operator*(float f) { return Color(r * f, g * f, b * f, a * f); }
			Color operator/(float f) { return Color(r / f, g / f, b / f, a / f); }

			inline bool Check(Color color)
			{
				return (this->r == color.r && this->g == color.g && this->b == color.b && this->a == color.a);
			}

			static inline Color Red()
			{
				return Color(1.0f, 0.0f, 0.0f, 1.0f);
			}
			
			static inline Color Blue()
			{
				return Color(0.0f, 1.0f, 0.0f, 1.0f);
			}
			
			static inline Color Green()
			{
				return Color(0.0f, 0.0f, 1.0f, 1.0f);
			}


		};

	}
}


#endif // !__MY_VECTOR_H__
