//#include <gtest/gtest.h>
//#include "extruder.h"
//#include "trapezoidmodeler.h"
//#include "vtkconverter.h"
//#include "visualizationwindow.h"

//// 测试完整流程：参数设置 -> 构建模型 -> 拉伸模型 -> 保存模型
//TEST(IntegrationTest, FullWorkflow_Valid) {
//    TrapezoidModeler modeler;
//    modeler.setParameters(40.0, 20.0, 20.0);
//    modeler.buildModel("2d_geometry");

//    Extruder extruder;
//    extruder.setExtrudeParameters(10.0);
//    TopoDS_Shape extrudedShape = extruder.extrude(modeler.getShape());

//    bool saveResult = modeler.saveModel("output.brep");

//    EXPECT_FALSE(extrudedShape.IsNull());
//    EXPECT_TRUE(saveResult);
//    EXPECT_EQ(modeler.getStatusInfo(), "Model saved successfully.");
//}

//// 测试模型显示
//TEST(IntegrationTest, Visualization_Valid) {
//    TrapezoidModeler modeler;
//    modeler.setParameters(40.0, 20.0, 20.0);
//    modeler.buildModel("2d_geometry");

//    VisualizationWindow window;
//    vtkSmartPointer<vtkPolyData> polyData = VtkConverter::convertToVtkHighLevel(modeler.getShape());
//    window.setModel(polyData);

//    EXPECT_TRUE(polyData->GetNumberOfPoints() > 0);
//}

//// 测试异常参数处理
//TEST(IntegrationTest, InvalidParameters) {
//    TrapezoidModeler modeler;
//    bool result = modeler.setParameters(-10.0, 20.0, 20.0);
//    EXPECT_FALSE(result);
//    EXPECT_EQ(modeler.getErrorInfo(), "Invalid parameters: width, height must be positive and theta must be between 0 and 90 degrees.");
//}

//// 测试保存失败
//TEST(IntegrationTest, SaveModel_Failure) {
//    TrapezoidModeler modeler;
//    modeler.setParameters(40.0, 20.0, 20.0);
//    modeler.buildModel("2d_geometry");

//    bool saveResult = modeler.saveModel("invalid_path.brep");
//    EXPECT_FALSE(saveResult);
//    EXPECT_EQ(modeler.getErrorInfo(), "Unsupported file format. Please use .brep extension.");
//}

//// 测试模型转换失败
//TEST(IntegrationTest, ModelConversion_Failure) {
//    TrapezoidModeler modeler;
//    modeler.setParameters(40.0, 20.0, 20.0);
//    modeler.buildModel("2d_geometry");

//    vtkSmartPointer<vtkPolyData> polyData = VtkConverter::convertToVtkHighLevel(TopoDS_Shape());
//    EXPECT_TRUE(polyData->GetNumberOfPoints() == 0);
//}
