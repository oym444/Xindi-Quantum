#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QMessageBox>
#include <QCommandLineParser>
#include <QDir>
#include <QFileInfo>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QCommandLineParser parser;
    parser.setApplicationDescription("CADBuilder");
    parser.addHelpOption();
    parser.addPositionalArgument("config", "Path to the configuration file.");

    parser.process(a);

    QString configPath;

    // 检查是否有命令行参数
    if (parser.positionalArguments().size() == 1) {
        configPath = parser.positionalArguments().first();
    } else {
        // 动态获取可执行文件路径
        QString exePath = QCoreApplication::applicationFilePath();
        QString exeDir = QFileInfo(exePath).absoluteDir().absolutePath();
        configPath = QDir(exeDir).filePath("config.json");
    }

    // 检查配置文件是否存在
    if (!QFile::exists(configPath)) {
        QMessageBox::critical(nullptr, "Error", "Configuration file not found.");
        return 1;
    }

    // 读取和解析配置文件
    QFile configFile(configPath);
    if (!configFile.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(nullptr, "Error", "Failed to open configuration file.");
        return 1;
    }

    QByteArray configData = configFile.readAll();
    QJsonParseError parseError;
    QJsonDocument configDoc = QJsonDocument::fromJson(configData, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        QMessageBox::critical(nullptr, "Error", "Failed to parse configuration file: " + QString(parseError.errorString()));
        return 1;
    }

    if (configDoc.isNull() || !configDoc.isObject()) {
        QMessageBox::critical(nullptr, "Error", "Invalid configuration file format.");
        return 1;
    }

    QJsonObject configObj = configDoc.object();

    // 提取参数
    double width = configObj["width"].toDouble();
    double height = configObj["height"].toDouble();
    double theta = configObj["theta"].toDouble();
    double extrudeDistance = configObj["extrude_distance"].toDouble();
    QString savePath = configObj["save_path"].toString();

    // 验证参数
    if (width <= 0 || height <= 0 || theta <= 0 || theta >= 90 || extrudeDistance <= 0) {
        QMessageBox::critical(nullptr, "Error", "Invalid parameters in configuration file.");
        return 1;
    }

    // 创建并显示主窗口
    MainWindow w;
    w.setParameters(width, height, theta, extrudeDistance, savePath);
    w.show();

    return a.exec();
}
