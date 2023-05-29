/** Statistic analyzer sourse file
 * 
 * Copyright (C) Gubaydullin Nurislam, School 21. All Rights Reserved
 * 
 * If the code of this project has helped you in any way,
 * please thank me with a cup of beer.
 * 
 * gmail:    gubaydullin.nurislam@gmail.com
 * telegram: @tonitaga
 */

#include "laboratoryview.h"
#include "ui_laboratoryview.h"

#include <QFileDialog>
#include <QMessageBox>

LaboratoryView::LaboratoryView(QWidget *parent) 
    : QMainWindow(parent) , ui(new Ui::LaboratoryView) , controller_(new s21::LaboratoryController) {
    ui->setupUi(this);
    ui->graph->setInteraction(QCP::iRangeZoom, true);
    ui->graph->setInteraction(QCP::iRangeDrag, true);
    setWindowIcon(QIcon(":/icon.png"));
    setWindowTitle("Statistic analyzer");
    setGraphTheme();
    setToolTips();
}

LaboratoryView::~LaboratoryView() {
    delete ui;
    delete controller_;
}

void LaboratoryView::on_loadStatisticPushButton_clicked() {
    auto file_path = QFileDialog::getOpenFileName(this, "Выберите файл с выборкой");
    if (file_path.isEmpty())
        return;
    controller_->readStatisticFromFile(file_path.toStdString());
    controller_->sortStatistic();
    if (controller_->statisticIsGood()) {
        statistic_is_read_ = true;
        QMessageBox::information(this, "Статус считанности", "Выборка была считана успешно!");
        setReadData();
    } else {
        QMessageBox::information(this, "Статус считанности", "Выборка была считана неудачно! (Некорректные данные в файле)");
    }
}

void LaboratoryView::on_intervalsBuildPushButton_clicked() {
    if (!statistic_is_read_)
        return;
    auto intervals_count = static_cast<statistic_size_type>(ui->intervals_count->value());
    controller_->buildStatisticIntervals(intervals_count);
    statistic_intervals_built_ = true;
    QMessageBox::information(this, "Интервалы", "Выборка была успешно разбита на интервалы!");
    buildStatisticGraph();
}

void LaboratoryView::on_getStatisticIntervalsPushButton_clicked() {
    if (!statistic_intervals_built_)
        return;
    auto file_path = QFileDialog::getSaveFileName(this, "Путь сохранение интервалов выборки");
    if (file_path.isEmpty())
        return;
    std::ofstream fout(file_path.toStdString());
    if (!fout.is_open())
        return;
    controller_->showStatisticIntervals(fout);
    QMessageBox::information(this, "Интервалы", "Интервалы были сохранены по пути: " + file_path);
    fout.close();
}

void LaboratoryView::on_saveSortedStatisticPushButton_clicked() {
    if (!statistic_is_read_)
        return;
    auto file_path = QFileDialog::getSaveFileName(this, "Путь сохранение отсортированной выборки");
    if (file_path.isEmpty())
        return;
    std::ofstream fout(file_path.toStdString());
    if (!fout.is_open())
        return;
    controller_->showStatistic(fout, '\n');
    QMessageBox::information(this, "Выборка", "Отсортированной выборка была сохранена по пути: " + file_path);
    fout.close();
}

void LaboratoryView::setReadData() {
    ui->label_statistic_size->setText(QString::number(controller_->getStatisticSize()));
    ui->label_statistic_mean->setText(QString::number(controller_->getStatisticMeanValue()));
    auto quadratic_deviation = controller_->getStatisticMeanQuadraticDeviationValue();
    ui->label_statistic_deviation->setText(QString::number(quadratic_deviation));
    ui->label_statistic_dispersion->setText(QString::number(std::pow(quadratic_deviation, 2)));
    ui->label_statistic_min_value->setText(QString::number(controller_->getStatisticMinimumValue()));
    ui->label_statistic_max_value->setText(QString::number(controller_->getStatisticMaximumValue()));
    ui->label_statistic_sum->setText(QString::number(controller_->getStatisticSum()));
}

void LaboratoryView::buildStatisticGraph() {
    if (!statistic_intervals_built_)
        return;
    auto [x1_points, y1_points] = controller_->getGraphStatisticData();
    if (x1_points.empty())
        return;
    ui->graph->clearGraphs();
    ui->graph->replot();
    double step = 0.05;
    ui->graph->xAxis->setRange(x1_points.front() - step, x1_points.back() + step);
    ui->graph->yAxis->setRange(0, *(std::max_element(y1_points.begin(), y1_points.end())) + step);

    ui->graph->addGraph()->setPen(QPen(QColor( 66, 135, 245)));
    ui->graph->graph(0)->setLineStyle(QCPGraph::lsLine);
    ui->graph->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 5));
    for (statistic_size_type current = 0, size = x1_points.size(); current != size; ++current)
        ui->graph->graph(0)->addData(x1_points[current], y1_points[current]);
    ui->graph->graph(0)->setName("Практическое распределение");
    ui->graph->replot();
}

void LaboratoryView::on_addNormalDistributionGraphPushButton_clicked() {
    if (!statistic_intervals_built_)
        return;
    if (!normal_distribution_is_built_) {
        auto [x2_points, y2_points] = controller_->getGraphNormalDistributionData();

        ui->graph->addGraph()->setPen(QPen(QColor(245, 66, 126)));
        for (statistic_size_type current = 0, size = x2_points.size(); current != size; ++current)
                ui->graph->graph(1)->addData(x2_points[current], y2_points[current]);
        ui->graph->graph(1)->setName("Нормальное распределение");
        ui->graph->replot();
        normal_distribution_is_built_ = true;
    } else {
        buildStatisticGraph();
        normal_distribution_is_built_ = false;
    }
}

void LaboratoryView::on_eraseStatisticValuePushButton_clicked() {
    if (!statistic_is_read_)
        return;
    normal_distribution_is_built_ = false;
    controller_->eraseStatisticValue(ui->erase_value_line_edit->text().toDouble());
    on_intervalsBuildPushButton_clicked();
    on_addNormalDistributionGraphPushButton_clicked();
    setReadData();
}

void LaboratoryView::setGraphTheme() {
    ui->graph->setBackground(QColor(47, 0, 74));
    ui->graph->xAxis->setTickLabelColor(QColor(255,255,255));
    ui->graph->xAxis->setBasePen(QPen(QColor(255,255,255)));
    ui->graph->xAxis->setLabelColor(QColor(255,255,255));
    ui->graph->xAxis->setTickPen(QPen(QColor(255,255,255)));
    ui->graph->xAxis->setSubTickPen(QColor(255,255,255));

    ui->graph->yAxis->setTickLabelColor(QColor(255,255,255));
    ui->graph->yAxis->setBasePen(QPen(QColor(255,255,255)));
    ui->graph->yAxis->setLabelColor(QColor(255,255,255));
    ui->graph->yAxis->setTickPen(QPen(QColor(255,255,255)));
    ui->graph->yAxis->setSubTickPen(QColor(255,255,255));
}

void LaboratoryView::setToolTips() {
    ui->loadStatisticPushButton->setToolTip("Загрузить выборку и файла");
    ui->saveSortedStatisticPushButton->setToolTip("Сохраняет загруженную выборку в файл в отсортированном виде");
    ui->intervalsBuildPushButton->setToolTip("Разбивает выборку на n-количество разных интервалов, и стоит практический график распределения");
    ui->getStatisticIntervalsPushButton->setToolTip("Сохраняет разбитую выботку на n-интервалов в файл");
    ui->addNormalDistributionGraphPushButton->setToolTip("Добавляет к графику практического распределения график нормального распределения");
}
