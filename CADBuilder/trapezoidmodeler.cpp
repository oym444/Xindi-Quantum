#include "trapezoidmodeler.h"
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepTools.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS_Face.hxx>
#include <cmath>


TrapezoidModeler::TrapezoidModeler() : m_width(0), m_height(0), m_theta(0) {}

bool TrapezoidModeler::setParameters(double width, double height, double theta) {
    // 参数检查
    if (width <= 0 || height <= 0 || theta <= 0 || theta >= 90) {
        m_errorInfo = "Invalid parameters: width, height must be positive and theta must be between 0 and 90 degrees.";
        return false;
    }

    m_width = width;
    m_height = height;
    m_theta = theta;
    m_errorInfo.clear();
    return true;
}

bool TrapezoidModeler::buildModel(const QString& name) {
    // 将角度转换为弧度
    double thetaRad = M_PI * m_theta / 180.0;

    // 计算顶点坐标
    double halfWidth = m_width / 2.0;
    double topWidth = m_width - 2 * m_height * tan(thetaRad);

    // 创建顶点
    BRepBuilderAPI_MakeVertex mv1(gp_Pnt(-halfWidth, 0, 0));
    BRepBuilderAPI_MakeVertex mv2(gp_Pnt(halfWidth, 0, 0));
    BRepBuilderAPI_MakeVertex mv3(gp_Pnt(halfWidth - topWidth/2, m_height, 0));
    BRepBuilderAPI_MakeVertex mv4(gp_Pnt(-halfWidth + topWidth/2, m_height, 0));

    // 检查顶点创建是否成功
    if (!mv1.IsDone() || !mv2.IsDone() || !mv3.IsDone() || !mv4.IsDone()) {
        m_errorInfo = "Failed to create vertices.";
        return false;
    }

    TopoDS_Vertex v1 = mv1.Vertex();
    TopoDS_Vertex v2 = mv2.Vertex();
    TopoDS_Vertex v3 = mv3.Vertex();
    TopoDS_Vertex v4 = mv4.Vertex();

    // 创建边
    BRepBuilderAPI_MakeEdge e1(v1, v2);
    BRepBuilderAPI_MakeEdge e2(v2, v3);
    BRepBuilderAPI_MakeEdge e3(v3, v4);
    BRepBuilderAPI_MakeEdge e4(v4, v1);

    // 检查边创建是否成功
    if (!e1.IsDone() || !e2.IsDone() || !e3.IsDone() || !e4.IsDone()) {
        m_errorInfo = "Failed to create edges.";
        return false;
    }

    TopoDS_Edge edge1 = e1.Edge();
    TopoDS_Edge edge2 = e2.Edge();
    TopoDS_Edge edge3 = e3.Edge();
    TopoDS_Edge edge4 = e4.Edge();

    // 创建线框
    BRepBuilderAPI_MakeWire wireMaker;
    wireMaker.Add(edge1);
    wireMaker.Add(edge2);
    wireMaker.Add(edge3);
    wireMaker.Add(edge4);

    if (!wireMaker.IsDone()) {
        m_errorInfo = "Failed to create wire.";
        return false;
    }

    TopoDS_Wire wire = wireMaker.Wire();

    // 创建面
    BRepBuilderAPI_MakeFace faceMaker(wire);

    if (!faceMaker.IsDone()) {
        m_errorInfo = "Failed to create face.";
        return false;
    }

    m_shape = faceMaker.Face();
    m_statusInfo = "Model built successfully.";
    return true;
}

TopoDS_Shape TrapezoidModeler::getShape() const {
    return m_shape;
}

void TrapezoidModeler::setShape(const TopoDS_Shape& shape) {
    m_shape = shape;
}

bool TrapezoidModeler::saveModel(const QString& filePath) {
    if (m_shape.IsNull()) {
        m_errorInfo = "No model to save.";
        return false;
    }

    // 检查文件路径是否为空
    if (filePath.isEmpty()) {
        m_errorInfo = "File path is empty.";
        return false;
    }

    // 确保文件路径是 .brep 格式
    if (!filePath.endsWith(".brep", Qt::CaseInsensitive)) {
        m_errorInfo = "Unsupported file format. Please use .brep extension.";
        return false;
    }

    // 将 QString 转换为 std::string
    std::string stdFilePath = filePath.toStdString();

    // 使用 OpenCASCADE 的 BRepTools::Write 函数保存模型到文件
    Standard_Boolean status = BRepTools::Write(m_shape, stdFilePath.c_str());

    if (!status) {
        m_errorInfo = "Failed to save model to file.";
        return false;
    }

    m_statusInfo = "Model saved successfully.";
    return true;
}

QString TrapezoidModeler::getErrorInfo() const {
    return m_errorInfo;
}

QString TrapezoidModeler::getStatusInfo() const {
    return m_statusInfo;
}
