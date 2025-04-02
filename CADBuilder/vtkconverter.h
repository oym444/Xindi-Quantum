/**
 * @file vtkconverter.h
 * @brief VTK 转换器头文件
 * @author [欧阳尹明]
 * @version 1.0
 * @date 2024-4-02
 */

#pragma once
#include <TopoDS_Shape.hxx>
#include <vtkPolyData.h>
#include <IVtkOCC_Shape.hxx>
#include <IVtkTools_ShapeDataSource.hxx>


/**
 * @brief VTK 转换器类
 */
class VtkConverter {
public:
    /**
     * @brief 使用高级 API 转换为 VTK 数据
     * @param shape OpenCASCADE 模型形状
     * @return 转换后的 VTK 数据
     */
    static vtkSmartPointer<vtkPolyData> convertToVtkHighLevel(const TopoDS_Shape& shape);

    /**
     * @brief 使用低级 API 转换为 VTK 数据
     * @param shape OpenCASCADE 模型形状
     * @return 转换后的 VTK 数据
     */
    static vtkSmartPointer<vtkPolyData> convertToVtkLowLevel(const TopoDS_Shape& shape);
};

