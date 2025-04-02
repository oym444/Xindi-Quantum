/**
 * @file visualizationwindow.h
 * @brief 可视化窗口头文件
 * @author [欧阳尹明]
 * @version 1.0
 * @date 2025-4-02
 */
#pragma once
#include <QMainWindow>
#include <QVTKOpenGLNativeWidget.h>
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkActor.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkCellPicker.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <QRect>


/**
 * @brief 可视化窗口类
 */
class VisualizationWindow : public QMainWindow {
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param parent 父控件
     */
    explicit VisualizationWindow(QWidget *parent = nullptr);

    /**
     * @brief 设置模型数据
     * @param polyData 模型数据
     */
    void setModel(vtkSmartPointer<vtkPolyData> polyData);

protected:
    /**
     * @brief 鼠标按下事件
     * @param event 鼠标事件
     */
    void mousePressEvent(QMouseEvent *event) override;


private:
    QVTKOpenGLNativeWidget *m_vtkWidget;
    vtkSmartPointer<vtkRenderer> m_renderer;
    vtkSmartPointer<vtkActor> m_modelActor;
    vtkSmartPointer<vtkPolyDataMapper> m_mapper;
    vtkSmartPointer<vtkActor> m_selectedActor;
    vtkSmartPointer<vtkCellPicker> m_picker;
};
