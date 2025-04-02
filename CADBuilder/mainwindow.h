/**
 * @file mainwindow.h
 * @brief 主窗口头文件
 * @author [欧阳尹明]
 * @version 1.0
 * @date 2025-4-2
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "trapezoidmodeler.h"
#include "extruder.h"
#include "commandparser.h"
#include "commandhandler.h"
#include "CommandInputDialog.h"
#include <QMainWindow>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QFileDialog>
#include <QComboBox>
#include "visualizationwindow.h"
#include <vtkRenderWindow.h>
#include <QVTKOpenGLNativeWidget.h>
#include <vtkActor.h>



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
 * @brief 主窗口类
 */
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    /**
     * @brief 构造函数
     * @param parent 父控件
     */
    MainWindow(QWidget *parent = nullptr);

    /**
     * @brief 析构函数
     */
    ~MainWindow();

    /**
     * @brief 设置参数
     * @param width 宽度
     * @param height 高度
     * @param theta 偏移角
     * @param extrudeDistance 拉伸距离
     * @param savePath 保存路径
     */
    void setParameters(double width, double height, double theta, double extrudeDistance, const QString& savePath);

private slots:
    /**
     * @brief 构建模型按钮点击事件
     */
    void onBuildModelClicked();

    /**
     * @brief 保存模型按钮点击事件
     */
    void onSaveModelClicked();

    /**
     * @brief 拉伸模型按钮点击事件
     */
    void onExtrudeClicked();

    /**
     * @brief 显示模型按钮点击事件
     */
    void onShowModelClicked();

    /**
     * @brief 命令按钮点击事件
     */
    void onCommandButtonClicked();

private:
    Ui::MainWindow *ui;
    TrapezoidModeler m_modeler;
    Extruder m_extruder;
    QLineEdit *m_widthEdit;
    QLineEdit *m_heightEdit;
    QLineEdit *m_thetaEdit;
    QLineEdit *m_extrudeDistanceEdit;
    QLineEdit *m_savePathEdit;
    QPushButton *m_buildButton;
    QPushButton *m_saveButton;
    QPushButton *m_extrudeButton;
    QPushButton *m_showButton;
    QPushButton *m_clearButton;
    QPushButton *m_commandButton;
    QLabel *m_statusLabel;
    vtkRenderWindow *m_renderWindow;
    QVTKOpenGLNativeWidget* m_vtkWidget;
    QComboBox* m_meshingCombo;
    VisualizationWindow *m_visualizationWindow;

    CommandParser *m_parser;
    CommandHandler *m_handler;
    CommandInputDialog* m_commandInputDlg;
};

#endif // MAINWINDOW_H
