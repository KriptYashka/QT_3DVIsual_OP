#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "logic.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QPainter>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_fileDialog_btn_clicked();

    void on_fileLoad_btn_clicked();

    void on_draw_btn_clicked();

    void on_rotate_x_btn_clicked();
    void on_rotate_y_btn_clicked();
    void on_rotate_z_btn_clicked();

    void on_offset_x_btn_clicked();
    void on_offset_y_btn_clicked();
    void on_offset_z_btn_clicked();

    void on_rotate_x_back_btn_clicked();
    void on_rotate_y_back_btn_clicked();
    void on_rotate_z_back_btn_clicked();

    void on_normalization_btn_clicked();

private:
    Ui::MainWindow *ui;
    QPixmap *pix = nullptr;
    QString fileName;

    void displayError(std::string message);

    void drawSurface(Line* lines, int count);
    void drawLine(QPainter* paint, Line* line);

    void rotationHandler(Axis axis, int direction = 1);

    void offsetHandler(Axis axis);
};
#endif // MAINWINDOW_H
