#include "extruder.h"
#include <BRepPrimAPI_MakePrism.hxx>

Extruder::Extruder() : m_distance(0) {}

bool Extruder::setExtrudeParameters(double distance) {
    // 参数检查
    if (distance <= 0) {
        m_errorInfo = "Invalid parameter: distance must be positive.";
        return false;
    }

    m_distance = distance;
    m_errorInfo.clear();
    return true;
}

TopoDS_Shape Extruder::extrude(const TopoDS_Shape& shape) {
    if (shape.IsNull()) {
        m_errorInfo = "Input shape is null.";
        m_statusInfo = "Extrude failed.";
        return TopoDS_Shape();
    }

    // 使用 BRepPrimAPI_MakePrism 进行拉伸
    BRepPrimAPI_MakePrism prism(shape, gp_Vec(0, 0, m_distance));
    if (!prism.IsDone()) {
        m_errorInfo = "Failed to extrude shape.";
        m_statusInfo = "Extrude failed.";
        return TopoDS_Shape();
    }

    m_statusInfo = "Extrude completed successfully.";
    return prism.Shape();
}

QString Extruder::getErrorInfo() const {
    return m_errorInfo;
}

QString Extruder::getStatusInfo() const {
    return m_statusInfo;
}
