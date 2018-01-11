#include "stdafx.h"
#include "CppUnitTest.h"

#include "../stdafx.h"
#include "../Triangle.h"
#include "../Ray.h"
#include "../BB3.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BBTest1
{
	TEST_CLASS(BBTest1)
	{
	public:

		TEST_METHOD(bb_Intersect1)
		{
			Ray r(glm::vec3(0.0f), glm::vec3(1.0f));
			r.initRay();

			BB3 bb;
			bb.min_ = glm::vec3(1.0f);
			bb.max_ = glm::vec3(2.0f);
			float near, far;
			bool hit = bb.Intersect(r, near,far);

			Assert::IsTrue(hit);
			Assert::AreEqual(sqrt(3.0f) * 2.0f, far, 1e-5f);
			Assert::AreEqual(sqrt(3.0f), near, 1e-5f);
		}

		TEST_METHOD(bb_Intersect2)
		{
			Ray r(glm::vec3(0.0f), glm::vec3(0.0f,0.0f,1.0f));
			r.initRay();

			BB3 bb;
			bb.min_ = glm::vec3(-1.0f);
			bb.max_ = glm::vec3(1.0f);
			float near, far;
			bool hit = bb.Intersect(r, near, far);

			Assert::IsTrue(hit);
			Assert::AreEqual(1.0f, far, 1e-5f);
			Assert::AreEqual(0.0f, near, 1e-5f);
		}

		TEST_METHOD(bb_Intersect3)
		{
			Ray r(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			r.initRay();

			BB3 bb;
			bb.min_ = glm::vec3(-1.0f, -1.0f,  1.0f);
			bb.max_ = glm::vec3(1.0f,1.0f, 2.0f);
			float near, far;
			bool hit = bb.Intersect(r, near, far);

			Assert::IsTrue(hit);
			Assert::AreEqual(2.0f, far, 1e-5f);
			Assert::AreEqual(1.0f, near, 1e-5f);
		}
		TEST_METHOD(bb_Intersect4)
		{
			Ray r(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			r.initRay();

			BB3 bb;
			bb.min_ = glm::vec3(-1.0f, -1.0f, -2.0f);
			bb.max_ = glm::vec3(1.0f, 1.0f, -1.0f);
			float near, far;
			bool hit = bb.Intersect(r, near, far);

			Assert::IsFalse(hit);
		}
		
		TEST_METHOD(bb_Intersect_too_far)
		{
			Ray r(glm::vec3(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
			r.initRay();
			r.tMax_ = 0.5;
			BB3 bb;
			bb.min_ = glm::vec3(1.0f, 1.0f, 1.0f);
			bb.max_ = glm::vec3(2.0f, 2.0f, 2.0f);
			float near, far;
			bool hit = bb.Intersect(r, near, far);

			Assert::IsFalse(hit);
		}

	};
}