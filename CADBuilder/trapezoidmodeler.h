/**
 * @file trapezoidmodeler.h
 * @brief 梯形建模器实现文件
 * @author [欧阳尹明]
 * @version 1.0
 * @date 2025-4-02
 */
#pragma once
#include <QString>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <TopoDS_Shape.hxx>

/**
 * @brief 梯形建模器类
 */
class TrapezoidModeler {
public:
    /**
     * @brief 构造函数
     */
    TrapezoidModeler();

    /**
     * @brief 设置等腰梯形参数
     * @param width 底边长度
     * @param height 高度
     * @param theta 腰与底边垂线的夹角（以度为单位）
     * @return 是否成功
     */
    bool setParameters(double width, double height, double theta);

    /**
     * @brief 构建等腰梯形模型
     * @param name 模型名称
     * @return 是否成功
     */
    bool buildModel(const QString& name);

    /**
     * @brief 获取模型
     * @return 模型形状
     */
    TopoDS_Shape getShape() const;

    /**
     * @brief 设置模型形状
     * @param shape 模型形状
     */
    void setShape(const TopoDS_Shape& shape);

    /**
     * @brief 保存构建的模型
     * @param filePath 保存文件路径
     * @return 是否成功
     */
    bool saveModel(const QString& filePath);

    /**
     * @brief 获取错误信息
     * @return 错误信息
     */
    QString getErrorInfo() const;

    /**
     * @brief 获取建模状态信息
     * @return 状态信息
     */
    QString getStatusInfo() const;

private:
    double m_width;         ///< 底边长度
    double m_height;        ///< 高度
    double m_theta;         ///< 腰与底边垂线的夹角（以度为单位）
    QString m_errorInfo;    ///< 错误信息
    QString m_statusInfo;   ///< 状态信息
    TopoDS_Shape m_shape;   ///< 梯形模型形状
};

