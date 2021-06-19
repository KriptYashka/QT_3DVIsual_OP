#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "logic.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
}

MainWindow::~MainWindow(){
    delete ui;
    delete pix;

    Request* request = new Request;
    request->operation = Operations::CLEAR;

    Response* response = execute(request);

    delete request;
    delete response;
}

void MainWindow::on_fileDialog_btn_clicked(){
    /* Запись пути к файлу */
    //fileName = QFileDialog::getOpenFileName(this, "Выберите файл", QDir::currentPath());
    fileName = "schools_exams.csv";
    ui->fileName_label->setText(fileName);

}

void MainWindow::on_fileLoad_btn_clicked(){
    /* Считывание файла и сохранение в LoadData в logic */
    Request* request_readfile = new Request;
    request_readfile->operation = Operations::READ_FILE;
    request_readfile->path = fileName.toStdString();


    Request* request_clear = new Request;
    request_clear->operation = Operations::CLEAR;

    Response* response_clear = execute(request_clear);
    Response* response_readfile = execute(request_readfile);


    if (!response_clear->done){
        show_error(response_clear->message);
    }
    delete request_readfile;
    delete response_clear;
    delete request_clear;
    delete response_readfile;
}

void MainWindow::on_draw_btn_clicked(){
    /* Отрисовка на QPix точек */
    Request* request = new Request;
    request->operation = Operations::DRAW;

    Response* response = execute(request);

    if (!response->done){
        show_error(response->message);
    } else {
        draw_surface(response->lines, response->line_count);
    }
    delete request;
    delete response;
}

void MainWindow::on_rotate_x_btn_clicked(){
    logic_rotation(Axis::X);
}

void MainWindow::on_rotate_y_btn_clicked(){
    logic_rotation(Axis::Y);
}

void MainWindow::on_rotate_z_btn_clicked(){
    logic_rotation(Axis::Z);
}

void MainWindow::on_rotate_x_back_btn_clicked(){
    logic_rotation(Axis::X, -1);
}

void MainWindow::on_rotate_y_back_btn_clicked(){
    logic_rotation(Axis::Y, -1);
}

void MainWindow::on_rotate_z_back_btn_clicked(){
    logic_rotation(Axis::Z, -1);
}

void MainWindow::on_offset_x_btn_clicked(){
    logic_offset(Axis::X);
}

void MainWindow::on_offset_y_btn_clicked(){
    logic_offset(Axis::Y);
}

void MainWindow::on_offset_z_btn_clicked(){
    logic_offset(Axis::Z);
}

void MainWindow::on_normalization_btn_clicked(){
    Request* request = new Request;
    request->operation = Operations::NORMALIZE;

    request->normalization[0] = atof(ui->normalization_a->toPlainText().toStdString().c_str());
    request->normalization[1] = atof(ui->normalization_b->toPlainText().toStdString().c_str());

    Response* response = execute(request);

    if (!response->done){
        show_error(response->message);
    } else {
        draw_surface(response->lines, response->line_count);
    }
    delete request;
    delete response;
}

void MainWindow::draw_surface(Line* lines, int count){
    delete pix;
    pix = new QPixmap(450, 450);
    QPainter paint(pix);
    paint.fillRect(0, 0, 450, 450, QBrush(QColor(Qt::GlobalColor::white)));
    paint.setPen(Qt::blue);

    for (int i = 0; i < count; i++){
        draw_line(&paint, &(lines[i]));
    }

    ui->draw_label->setPixmap(*pix);
}

void MainWindow::draw_line(QPainter* paint, Line* line){
    paint->drawLine(line->point_first.x, line->point_first.y, line->point_second.x, line->point_second.y);
}

void MainWindow::show_error(string message){
    QMessageBox::about(this, "Ошибка", QString::fromStdString(message));
}

void MainWindow::logic_rotation(Axis axis, int direction){
    Request* request = new Request;

    request->operation = Operations::ROTATE;
    request->axis = axis;

    switch (axis){
        case Axis::X:
            request->rotation_angle = atof(ui->rotation_x->toPlainText().toStdString().c_str());;
            break;
        case Axis::Y:
            request->rotation_angle = atof(ui->rotation_y->toPlainText().toStdString().c_str());;
            break;
        case Axis::Z:
            request->rotation_angle = atof(ui->rotation_z->toPlainText().toStdString().c_str());;
            break;
    }

    request->rotation_angle *= direction;

    Response* response = execute(request);

    if (!response->done){
        show_error(response->message);
    } else {
        draw_surface(response->lines, response->line_count);
    }

    delete request;
    delete response;
}

void MainWindow::logic_offset(Axis axis){
    Request* request = new Request;

    request->operation = Operations::MOVE;
    request->axis = axis;

    switch (axis){
        case Axis::X:
            request->offset_value = atof(ui->offset_x->toPlainText().toStdString().c_str());;
            break;
        case Axis::Y:
            request->offset_value = atof(ui->offset_y->toPlainText().toStdString().c_str());;
            break;
        case Axis::Z:
            request->offset_value = atof(ui->offset_z->toPlainText().toStdString().c_str());;
            break;
    }

    Response* response = execute(request);

    if (!response->done){
        show_error(response->message);
    } else {
        draw_surface(response->lines, response->line_count);
    }
    delete request;
    delete response;
}
