//#include "pch.h"
//#include "ShaderFragment.h"
//#include "ShaderVertex.h"

//template<typename T>
//class ShaderTest : testing::Test {
//public:
//	T obj;
//};
//
//using MyTypes = ::testing::Types<ShaderVertex, ShaderFragment>;
//TYPED_TEST_SUITE(ShaderTest, MyTypes);
//
//TYPED_TEST(ShaderTest, ShaderDoesNotThrowOnCorrectInput) {
//	const char* vertexShaderSource = "#version 330 core\n"
//		"layout (location = 0) in vec3 aPos;\n"
//		"void main()\n"
//		"{\n"
//		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
//		"}\0";
//	ASSERT_NO_THROW({
//		TypeParam instance(vertexShaderSource);
//		});
//}