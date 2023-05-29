/** Statistic analyzer header file
 * 
 * Copyright (C) Gubaydullin Nurislam, School 21. All Rights Reserved
 * 
 * If the code of this project has helped you in any way,
 * please thank me with a cup of beer.
 * 
 * gmail:    gubaydullin.nurislam@gmail.com
 * telegram: @tonitaga
 */

#ifndef STATISTIC_ANALYZER_VIEW_LABORATORYVIEW_H_
#define STATISTIC_ANALYZER_VIEW_LABORATORYVIEW_H_

#include <QMainWindow>

#include "../Controller/laboratory_controller.h"


QT_BEGIN_NAMESPACE
namespace Ui { class LaboratoryView; }
QT_END_NAMESPACE

class LaboratoryView final : public QMainWindow {
    Q_OBJECT

public:
    LaboratoryView(QWidget *parent = nullptr);

    LaboratoryView(const LaboratoryView &) = delete;
    LaboratoryView(LaboratoryView &&) = delete;
    LaboratoryView &operator=(const LaboratoryView &) = delete;
    LaboratoryView &operator=(LaboratoryView &&) = delete;

    ~LaboratoryView();


private slots:
    void on_loadStatisticPushButton_clicked();
    void on_intervalsBuildPushButton_clicked();
    void on_getStatisticIntervalsPushButton_clicked();
    void on_saveSortedStatisticPushButton_clicked();
    void on_addNormalDistributionGraphPushButton_clicked();
    void on_eraseStatisticValuePushButton_clicked();

private:
    Ui::LaboratoryView *ui;
    s21::LaboratoryController *controller_;

    bool statistic_is_read_ = false;
    bool statistic_intervals_built_ = false;
    bool normal_distribution_is_built_ = false;

    void setReadData();
    void buildStatisticGraph();
    void setGraphTheme();
    void setToolTips();

    using statistic_type = s21::LaboratoryController::statistic_type;
    using statistic_value_type = s21::LaboratoryController::statistic_value_type;
    using statistic_size_type = s21::LaboratoryController::statistic_size_type;
};

#endif // !STATISTIC_ANALYZER_VIEW_LABORATORYVIEW_H_
