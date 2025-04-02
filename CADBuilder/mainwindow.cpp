#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <QVTKOpenGLNativeWidget.h>

#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>
#include <vtkNew.h>
#include "vtkconverter.h"           // 包含转换器头文件
#include "commandinputdialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // 设置窗口布局
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);

    // 创建输入参数组
    QGroupBox* inputGroup = new QGroupBox("Trapezoid Parameters", centralWidget);
    QVBoxLayout* inputLayout = new QVBoxLayout(inputGroup);

    // 宽度输入
    QHBoxLayout* widthLayout = new QHBoxLayout();
    widthLayout->addWidget(new QLabel("Width:", inputGroup));
    m_widthEdit = new QLineEdit(inputGroup);
    widthLayout->addWidget(m_widthEdit);
    inputLayout->addLayout(widthLayout);

    // 高度输入
    QHBoxLayout* heightLayout = new QHBoxLayout();
    heightLayout->addWidget(new QLabel("Height:", inputGroup));
    m_heightEdit = new QLineEdit(inputGroup);
    heightLayout->addWidget(m_heightEdit);
    inputLayout->addLayout(heightLayout);

    // 角度输入
    QHBoxLayout* thetaLayout = new QHBoxLayout();
    thetaLayout->addWidget(new QLabel("Theta (degrees):", inputGroup));
    m_thetaEdit = new QLineEdit(inputGroup);
    thetaLayout->addWidget(m_thetaEdit);
    inputLayout->addLayout(thetaLayout);

    // 拉伸距离输入
    QHBoxLayout* extrudeDistanceLayout = new QHBoxLayout();
    extrudeDistanceLayout->addWidget(new QLabel("Extrude Distance:", inputGroup));
    m_extrudeDistanceEdit = new QLineEdit(inputGroup);
    extrudeDistanceLayout->addWidget(m_extrudeDistanceEdit);
    inputLayout->addLayout(extrudeDistanceLayout);

    // 构建按钮
    m_buildButton = new QPushButton("Build Model", inputGroup);
    connect(m_buildButton, &QPushButton::clicked, this, &MainWindow::onBuildModelClicked);
    inputLayout->addWidget(m_buildButton);

    // 拉伸按钮
    m_extrudeButton = new QPushButton("Extrude Model", inputGroup);
    connect(m_extrudeButton, &QPushButton::clicked, this, &MainWindow::onExtrudeClicked);
    inputLayout->addWidget(m_extrudeButton);

    // 保存路径输入
    QHBoxLayout* saveLayout = new QHBoxLayout();
    saveLayout->addWidget(new QLabel("Save Path:", inputGroup));
    m_savePathEdit = new QLineEdit(inputGroup);
    saveLayout->addWidget(m_savePathEdit);
    QPushButton* browseButton = new QPushButton("Browse", inputGroup);
    connect(browseButton, &QPushButton::clicked, [this]() {
        QString filePath = QFileDialog::getSaveFileName(this, "Save Model", "", "BREP Files (*.brep)");
        if (!filePath.isEmpty()) {
            m_savePathEdit->setText(filePath);
        }
    });
    saveLayout->addWidget(browseButton);
    inputLayout->addLayout(saveLayout);

    // 保存按钮
    m_saveButton = new QPushButton("Save Model", inputGroup);
    connect(m_saveButton, &QPushButton::clicked, this, &MainWindow::onSaveModelClicked);
    inputLayout->addWidget(m_saveButton);

    // 显示按钮
    m_showButton = new QPushButton("Show Model", inputGroup);
    connect(m_showButton, &QPushButton::clicked, this, &MainWindow::onShowModelClicked);
    inputLayout->addWidget(m_showButton);

    // 添加离散化方式选择框
    m_meshingCombo = new QComboBox(inputGroup);  // inputGroup 是父控件
    m_meshingCombo->addItem("High-Level API");
    m_meshingCombo->addItem("Low-Level API");
    inputLayout->addWidget(m_meshingCombo);      // 将下拉框添加到输入布局
    m_visualizationWindow = nullptr;


    // 命令按钮
    m_commandButton = new QPushButton("Command Input", inputGroup);
    connect(m_commandButton, &QPushButton::clicked, this, &MainWindow::onCommandButtonClicked);
    inputLayout->addWidget(m_commandButton);
//    // 清空按钮
//    m_clearButton = new QPushButton("Clear", inputGroup);
//    connect(m_clearButton, &QPushButton::clicked, this, &MainWindow::onClearClicked);
//    inputLayout->addWidget(m_clearButton);

    // 状态标签
    m_statusLabel = new QLabel("Ready", inputGroup);
    inputLayout->addWidget(m_statusLabel);

    mainLayout->addWidget(inputGroup);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    m_commandInputDlg = new CommandInputDialog(m_modeler, m_extruder, this);
}

MainWindow::~MainWindow() {
    delete ui;
    m_renderWindow->Delete();
}

void MainWindow::setParameters(double width, double height, double theta, double extrudeDistance, const QString& savePath) {
    m_widthEdit->setText(QString::number(width));
    m_heightEdit->setText(QString::number(height));
    m_thetaEdit->setText(QString::number(theta));
    m_extrudeDistanceEdit->setText(QString::number(extrudeDistance));
    m_savePathEdit->setText(savePath);
}

void MainWindow::onBuildModelClicked() {
    bool ok;
    double width = m_widthEdit->text().toDouble(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Input Error", "Invalid width value.");
        return;
    }

    double height = m_heightEdit->text().toDouble(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Input Error", "Invalid height value.");
        return;
    }

    double theta = m_thetaEdit->text().toDouble(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Input Error", "Invalid theta value.");
        return;
    }

    if (!m_modeler.setParameters(width, height, theta)) {
        QMessageBox::warning(this, "Parameter Error", m_modeler.getErrorInfo());
        return;
    }

    if (!m_modeler.buildModel("2d_geometry")) {
        QMessageBox::warning(this, "Build Error", m_modeler.getErrorInfo());
        return;
    }

    m_statusLabel->setText(m_modeler.getStatusInfo());
}

void MainWindow::onExtrudeClicked() {
    bool ok;
    double distance = m_extrudeDistanceEdit->text().toDouble(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Input Error", "Invalid extrude distance value.");
        return;
    }

    if (!m_extruder.setExtrudeParameters(distance)) {
        QMessageBox::warning(this, "Extrude Error", m_extruder.getErrorInfo());
        return;
    }

    TopoDS_Shape shape = m_extruder.extrude(m_modeler.getShape());
    if (shape.IsNull()) {
        QMessageBox::warning(this, "Extrude Error", m_extruder.getErrorInfo());
        return;
    }

    m_modeler.setShape(shape);
    m_statusLabel->setText(m_extruder.getStatusInfo());
}

void MainWindow::onSaveModelClicked() {
    QString filePath = m_savePathEdit->text();
    if (filePath.isEmpty()) {
        QMessageBox::warning(this, "Save Error", "Please select a save path.");
        return;
    }

    if (!m_modeler.saveModel(filePath)) {
        QMessageBox::warning(this, "Save Error", m_modeler.getErrorInfo());
        return;
    }

    m_statusLabel->setText(m_modeler.getStatusInfo());
}

void MainWindow::onShowModelClicked() {
    // 在VTK窗口中显示模型
    // 实现VTK渲染逻辑
    // m_statusLabel->setText("Model displayed.");
    if (m_modeler.getShape().IsNull()) {
        QMessageBox::warning(this, "Error", "No model to display!");
        return;
    }

    // 根据选择的方式转换数据
    vtkSmartPointer<vtkPolyData> polyData;
    if (m_meshingCombo->currentIndex() == 0) {
        polyData = VtkConverter::convertToVtkHighLevel(m_modeler.getShape());
    } else {
        polyData = VtkConverter::convertToVtkLowLevel(m_modeler.getShape());
    }

    if (!polyData) {
        QMessageBox::warning(this, "显示错误", "模型数据转换失败");
        return;
    }

    // 创建新的窗口显示模型
    if (!m_visualizationWindow) {
        m_visualizationWindow = new VisualizationWindow(this);
    }
    // 转换数据并显示
    auto polyDatas = VtkConverter::convertToVtkHighLevel(m_modeler.getShape());
    if (polyData && polyData->GetNumberOfPoints() > 0) {
        m_visualizationWindow->setModel(polyData);
        m_visualizationWindow->show();

        // 强制激活 OpenGL 上下文
        QApplication::processEvents();
        m_visualizationWindow->activateWindow();
        m_visualizationWindow->raise();
    } else {
        QMessageBox::warning(this, "Error", "Failed to convert model for display.");
    }

}

void MainWindow::onCommandButtonClicked() {
//    CommandInputDialog dialog(m_modeler, m_extruder, this);
    m_commandInputDlg->show();
}

