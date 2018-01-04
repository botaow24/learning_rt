#include "stdafx.h"
#include "CppUnitTest.h"

#include "../stdafx.h"
#include "../Triangle.h"
#include "../Ray.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(Triangle_Intersect1)
		{
			Ray r(glm::vec3(0.0f), glm::vec3(1.0f));
			r.initRay();
			glm::vec3  tri_p[3];
			tri_p[0] = glm::vec3(1.0f,0.0f,0.0f);
			tri_p[1] = glm::vec3(0.0f, 1.0f, 0.0f);
			tri_p[2] = glm::vec3(0.0f, 0.0f, 1.0f);
			glm::vec3 beo;
			float thint;
			bool result = Triangle::Intersect(r,tri_p, beo, thint);
			Assert::IsTrue(result);
			Assert::AreEqual(sqrt(3.0f) / 3 , thint , 1e-5f);
			// TODO: Your test code here
	}	
		TEST_METHOD(Triangle_Intersect2)
		{
			Ray r(glm::vec3(0.0f), glm::vec3(1.0f,0.0f,0.0f));
			r.initRay();
			glm::vec3  tri_p[3];
			tri_p[0] = glm::vec3(2.0f, 0.0f, 0.0f);
			tri_p[1] = glm::vec3(0.0f, 2.0f, 0.0f);
			tri_p[2] = glm::vec3(0.0f, 0.0f, 2.0f);
			glm::vec3 beo;
			float thint;
			bool result = Triangle::Intersect(r, tri_p, beo, thint);
			Assert::IsTrue(result);
			Assert::AreEqual(2.0f, thint, 1e-5f);
			// TODO: Your test code here
		}

		TEST_METHOD(Triangle_Intersect3)
		{
			Ray r(glm::vec3(0.0f), glm::vec3(-0.1f, 0.0f, 0.0f));
			r.initRay();
			glm::vec3  tri_p[3];
			tri_p[0] = glm::vec3(1.0f, 0.0f, 0.0f);
			tri_p[1] = glm::vec3(0.0f, 1.0f, 0.0f);
			tri_p[2] = glm::vec3(0.0f, 0.0f, 1.0f);
			glm::vec3 beo;
			float thint;
			bool result = Triangle::Intersect(r, tri_p, beo, thint);
			Assert::IsFalse(result);
			// TODO: Your test code here
		}

		TEST_METHOD(Triangle_Intersect4)
		{
			Ray r(glm::vec3(0.0f), glm::vec3(1.0f, 1.0f, 0.0f));
			r.initRay();
			glm::vec3  tri_p[3];
			tri_p[0] = glm::vec3(1.0f, 0.0f, 0.0f);
			tri_p[1] = glm::vec3(0.0f, 1.0f, 0.0f);
			tri_p[2] = glm::vec3(0.0f, 0.0f, 1.0f);
			glm::vec3 beo;
			float thint;
			bool result = Triangle::Intersect(r, tri_p, beo, thint);
			Assert::IsTrue(result);
			Assert::AreEqual(sqrt(2.0f)/2, thint, 1e-5f);
			// TODO: Your test code here
		}
	};
}