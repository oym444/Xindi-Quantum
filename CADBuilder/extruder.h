/**
 * @file extruder.h
 * @brief 拉伸器头文件
 * @author [欧阳尹明]
 * @version 1.0
 * @date 2025-4-02
 */
#pragma once
#include <QString>
#include <TopoDS_Shape.hxx>

/**
 * @brief 拉伸器类
 */
class Extruder {
public:
    /**
     * @brief 构造函数
     */
    Extruder();

    /**
     * @brief 设置拉伸参数
     * @param distance 拉伸距离
     * @return 是否成功
     */
    bool setExtrudeParameters(double distance);

    /**
     * @brief 执行拉伸操作
     * @param shape 输入的二维模型形状
     * @return 拉伸后的三维模型形状
     */
    TopoDS_Shape extrude(const TopoDS_Shape& shape);

    /**
     * @brief 获取错误信息
     * @return 错误信息
     */
    QString getErrorInfo() const;

    /**
     * @brief 获取拉伸状态信息
     * @return 状态信息
     */
    QString getStatusInfo() const;

private:
    double m_distance;      ///< 拉伸距离
    QString m_errorInfo;    ///< 错误信息
    QString m_statusInfo;   ///< 状态信息
};

