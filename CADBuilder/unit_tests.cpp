#include <gtest/gtest.h>
#include "trapezoidmodeler.h"
#include "extruder.h"
#include "vtkconverter.h"
#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <TopoDS_Shape.hxx>
#include <QString>

// 测试 TrapezoidModeler::setParameters 函数
TEST(TrapezoidModelerTest, SetParameters) {
    TrapezoidModeler modeler;
    // 创建合适的参数
    double width = 10.0;
    double height = 20.0;
    double theta = 30.0;
    bool result = modeler.setParameters(width, height, theta);
    EXPECT_TRUE(result);
}

// 测试 TrapezoidModeler::buildModel 函数
TEST(TrapezoidModelerTest, BuildModel) {
    TrapezoidModeler modeler;
    double width = 10.0;
    double height = 20.0;
    double theta = 30.0;
    modeler.setParameters(width, height, theta);
    QString modelName = "test_model";
    bool result = modeler.buildModel(modelName);
    EXPECT_TRUE(result);
}

// 测试 TrapezoidModeler::saveModel 函数
TEST(TrapezoidModelerTest, SaveModel) {
    TrapezoidModeler modeler;
    double width = 10.0;
    double height = 20.0;
    double theta = 30.0;
    modeler.setParameters(width, height, theta);
    QString modelName = "test_model";
    modeler.buildModel(modelName);
    QString filePath = "test_model.brep";
    bool result = modeler.saveModel(filePath);
    EXPECT_TRUE(result);
}

// 测试 TrapezoidModeler::getStatusInfo 函数
TEST(TrapezoidModelerTest, GetStatusInfo) {
    TrapezoidModeler modeler;
    double width = 10.0;
    double height = 20.0;
    double theta = 30.0;
    modeler.setParameters(width, height, theta);
    QString modelName = "test_model";
    modeler.buildModel(modelName);
    QString filePath = "test_model.brep";
    modeler.saveModel(filePath);
    QString status = modeler.getStatusInfo();
    EXPECT_FALSE(status.isEmpty());
}

// 测试 TrapezoidModeler::getErrorInfo 函数
TEST(TrapezoidModelerTest, GetErrorInfo) {
    TrapezoidModeler modeler;
    double invalidWidth = -10.0;
    double height = 20.0;
    double theta = 30.0;
    modeler.setParameters(invalidWidth, height, theta);
    QString error = modeler.getErrorInfo();
    EXPECT_FALSE(error.isEmpty());
}

// 测试 Extruder::extrude 函数
TEST(ExtruderTest, Extrude) {
    Extruder extruder;
    TrapezoidModeler modeler;
    double width = 10.0;
    double height = 20.0;
    double theta = 30.0;
    modeler.setParameters(width, height, theta);
    modeler.buildModel("test_model");
    TopoDS_Shape shape = modeler.getShape();
    // 设置拉伸参数
    double extrudeDistance = 5.0;
    extruder.setExtrudeParameters(extrudeDistance);
    TopoDS_Shape extrudedShape = extruder.extrude(shape);
    EXPECT_FALSE(extrudedShape.IsNull());
}

// 测试 VtkConverter::convertToVtkHighLevel 函数
TEST(VtkConverterTest, ConvertToVtkHighLevel) {
    VtkConverter converter;
    TrapezoidModeler modeler;
    double width = 10.0;
    double height = 20.0;
    double theta = 30.0;
    modeler.setParameters(width, height, theta);
    modeler.buildModel("test_model");
    TopoDS_Shape shape = modeler.getShape();
    vtkSmartPointer<vtkPolyData> polyData = converter.convertToVtkHighLevel(shape);
    EXPECT_TRUE(polyData != nullptr);
}

// 测试 VtkConverter::convertToVtkLowLevel 函数
TEST(VtkConverterTest, ConvertToVtkLowLevel) {
    VtkConverter converter;
    TrapezoidModeler modeler;
    double width = 10.0;
    double height = 20.0;
    double theta = 30.0;
    modeler.setParameters(width, height, theta);
    modeler.buildModel("test_model");
    TopoDS_Shape shape = modeler.getShape();
    vtkSmartPointer<vtkPolyData> polyData = converter.convertToVtkLowLevel(shape);
    EXPECT_TRUE(polyData != nullptr);
}

// 测试完整流程：参数设置 -> 构建模型 -> 拉伸模型 -> 保存模型
TEST(IntegrationTest, FullWorkflow_Valid) {
    TrapezoidModeler modeler;
    double width = 40.0;
    double height = 20.0;
    double theta = 20.0;
    modeler.setParameters(width, height, theta);
    QString modelName = "2d_geometry";
    modeler.buildModel(modelName);

    Extruder extruder;
    double extrudeDistance = 10.0;
    extruder.setExtrudeParameters(extrudeDistance);
    TopoDS_Shape extrudedShape = extruder.extrude(modeler.getShape());

    QString filePath = "output.brep";
    bool saveResult = modeler.saveModel(filePath);

    EXPECT_FALSE(extrudedShape.IsNull());
    EXPECT_TRUE(saveResult);
    EXPECT_EQ(modeler.getStatusInfo(), "Model saved successfully.");
}

// 测试异常参数处理
TEST(IntegrationTest, InvalidParameters) {
    TrapezoidModeler modeler;
    double invalidWidth = -10.0;
    double height = 20.0;
    double theta = 20.0;
    bool result = modeler.setParameters(invalidWidth, height, theta);
    EXPECT_FALSE(result);
    EXPECT_EQ(modeler.getErrorInfo(), "Invalid parameters: width, height must be positive and theta must be between 0 and 90 degrees.");
}

// 测试保存失败
TEST(IntegrationTest, SaveModel_Failure) {
    TrapezoidModeler modeler;
    double width = 40.0;
    double height = 20.0;
    double theta = 20.0;
    modeler.setParameters(width, height, theta);
    QString modelName = "2d_geometry";
    modeler.buildModel(modelName);

    QString invalidFilePath = "invalid_path.stl";
    bool saveResult = modeler.saveModel(invalidFilePath);
    EXPECT_FALSE(saveResult);
    EXPECT_EQ(modeler.getErrorInfo(), "Unsupported file format. Please use .brep extension.");
}

// 测试模型转换失败
TEST(IntegrationTest, ModelConversion_Failure) {
    TrapezoidModeler modeler;
    double width = 40.0;
    double height = 20.0;
    double theta = 20.0;
    modeler.setParameters(width, height, theta);
    QString modelName = "2d_geometry";
    modeler.buildModel(modelName);

    vtkSmartPointer<vtkPolyData> polyData = VtkConverter::convertToVtkHighLevel(TopoDS_Shape());
    EXPECT_TRUE(polyData->GetNumberOfPoints() == 0);
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
