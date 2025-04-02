#include "visualizationwindow.h"
#include <vtkNew.h>
#include <vtkCamera.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindow.h>
#include <vtkProperty.h>
#include <vtkCollection.h>
#include <vtkCoordinate.h>
#include <vtkBoundingBox.h>
#include <QDebug>
#include <QMouseEvent>
#include <QPainter>


VisualizationWindow::VisualizationWindow(QWidget *parent)
    : QMainWindow(parent)  {
    m_vtkWidget = new QVTKOpenGLNativeWidget(this);
    setCentralWidget(m_vtkWidget);

    m_renderer = vtkSmartPointer<vtkRenderer>::New();
    m_renderer->SetBackground(0.2, 0.3, 0.4);
    m_vtkWidget->renderWindow()->AddRenderer(m_renderer);

    m_modelActor = vtkSmartPointer<vtkActor>::New();
    m_mapper = vtkSmartPointer<vtkPolyDataMapper>::New(); // 初始化成员 Mapper
    m_modelActor->SetMapper(m_mapper); // 绑定到 Actor
    m_renderer->AddActor(m_modelActor);

    m_vtkWidget->renderWindow()->SetMultiSamples(0);
    m_vtkWidget->renderWindow()->Render();

    // 初始化拾取器
    m_picker = vtkSmartPointer<vtkCellPicker>::New();
    m_picker->SetTolerance(0.001);

    // 设置交互器风格
    vtkNew<vtkInteractorStyleTrackballCamera> style;
    m_vtkWidget->interactor()->SetInteractorStyle(style);

    //启用鼠标事件处理
    m_vtkWidget->setMouseTracking(true);
}


void VisualizationWindow::setModel(vtkSmartPointer<vtkPolyData> polyData) {
    if (!polyData) {
        std::cerr << "No valid polyData to display." << std::endl;
        return;
    }

    std::cout << "polyData points: " << polyData->GetNumberOfPoints() << std::endl;
    std::cout << "polyData cells: " << polyData->GetNumberOfCells() << std::endl;


    // 更新现有 Mapper 的数据
    m_mapper->SetInputData(polyData);
    m_mapper->Update();

    // 重置渲染器相机以适配新数据
    m_renderer->ResetCamera();
    m_vtkWidget->renderWindow()->Render();
}

void VisualizationWindow::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        int x = event->pos().x();
        int y = event->pos().y();
        int* size = m_vtkWidget->renderWindow()->GetSize();
        y = size[1] - y - 1;

        // (a) 屏幕坐标
        qDebug() << "(a) 屏幕坐标: (" << x << "," << y << ")";

        // 获取当前相机参数
        vtkCamera* camera = m_renderer->GetActiveCamera();
        double cameraPos[3], cameraFocal[3];
        camera->GetPosition(cameraPos);     // 相机位置
        camera->GetFocalPoint(cameraFocal); // 焦点位置

        // (b) 世界坐标：近裁剪面位置（z=0）
        double nearWorldPos[3];
        m_renderer->SetDisplayPoint(x, y, 0); // z=0 表示近裁剪面
        m_renderer->DisplayToWorld();
        m_renderer->GetWorldPoint(nearWorldPos);
        qDebug() << "(b) 近裁剪面坐标: ("
                 << nearWorldPos[0] << ","
                 << nearWorldPos[1] << ","
                 << nearWorldPos[2] << ")";

        // (d) 射线起点和方向：从相机到远裁剪面
        double farWorldPos[3];
        m_renderer->SetDisplayPoint(x, y, 1); // z=1 表示远裁剪面
        m_renderer->DisplayToWorld();
        m_renderer->GetWorldPoint(farWorldPos);

        qDebug() << "(d) 射线起点（相机位置）: ("
                 << cameraPos[0] << ","
                 << cameraPos[1] << ","
                 << cameraPos[2] << ")";
        qDebug() << "    射线方向向量: ("
                 << farWorldPos[0] - cameraPos[0] << ","
                 << farWorldPos[1] - cameraPos[1] << ","
                 << farWorldPos[2] - cameraPos[2] << ")";

        // 执行拾取操作
        m_picker->Pick(x, y, 0, m_renderer);

        if (m_picker->GetActor()) {
            // (c) 交点坐标：模型表面交点
            double* intersect = m_picker->GetPickPosition();
            qDebug() << "(c) 交点坐标: ("
                     << intersect[0] << ","
                     << intersect[1] << ","
                     << intersect[2] << ")";

            // 高亮选中的Actor
            if (m_selectedActor) {
                m_selectedActor->GetProperty()->SetColor(1, 1, 1); // 恢复原色
            }
            m_selectedActor = m_picker->GetActor();
            m_selectedActor->GetProperty()->SetColor(1, 0, 0); // 设置为红色
        } else {
            // 点击空白处取消高亮
            if (m_selectedActor) {
                m_selectedActor->GetProperty()->SetColor(1, 1, 1);
                m_selectedActor = nullptr;
            }
            qDebug() << "未选中任何物体";
        }

        m_vtkWidget->renderWindow()->Render();
    }
    QMainWindow::mousePressEvent(event);
}
