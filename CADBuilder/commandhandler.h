/**
 * @file commandhandler.h
 * @brief 命令处理器头文件
 * @author [欧阳尹明]
 * @version 1.0
 * @date 2025-4-2
 */

#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H

#include <QString>
#include <QMap>
#include <QMessageBox>
#include "trapezoidmodeler.h"
#include "extruder.h"
#include "visualizationwindow.h"
#include "vtkconverter.h"

/**
 * @brief 命令处理器类，用于处理用户输入的命令
 */
class CommandHandler {
public:
    /**
     * @brief 处理用户输入的命令
     * @param cmd 命令名称
     * @param params 命令参数
     * @param modeler 梯形建模器
     * @param extruder 拉伸器
     * @param visualizationWindow 可视化窗口
     * @return 命令执行是否成功
     */
    bool handleCommand(const QString& cmd, const QMap<QString, QString>& params, TrapezoidModeler& modeler, Extruder& extruder, VisualizationWindow* visualizationWindow) {
        if (cmd == "trapezoid") {
            QString name = params["-name"];
            double width = params["-width"].toDouble();
            double height = params["-height"].toDouble();
            double theta = params["-theta"].toDouble();

            if (modeler.setParameters(width, height, theta) && modeler.buildModel(name)) {
                return true;
            }
        } else if (cmd == "extrude") {
            QString name = params["-name"];
            QString source = params["-source"];
            double distance = params["-distance"].toDouble();

            if (extruder.setExtrudeParameters(distance)) {
                TopoDS_Shape shape = extruder.extrude(modeler.getShape());
                if (!shape.IsNull()) {
                    modeler.setShape(shape);
                    return true;
                }
            }
        } else if (cmd == "clear") {
            // 清空所有模型
            modeler.setShape(TopoDS_Shape());
            return true;
        } else if (cmd == "show") {
            // 显示所有模型
            if (modeler.getShape().IsNull()) {
                std::cerr << "No model to display!" << std::endl;
                return false;
            }

            // 根据选择的方式转换数据
            vtkSmartPointer<vtkPolyData> polyData;
            polyData = VtkConverter::convertToVtkHighLevel(modeler.getShape());


            if (!polyData) {
                std::cerr << "模型数据转换失败" << std::endl;
                return false;
            }

            if (polyData && polyData->GetNumberOfPoints() > 0) {
                visualizationWindow->setModel(polyData);
                visualizationWindow->show();
                return true;
            } else {
                std::cerr << "Failed to convert model for display." << std::endl;
                return false;
            }

        }
        return false;
    }
};

#endif // COMMANDHANDLER_H
