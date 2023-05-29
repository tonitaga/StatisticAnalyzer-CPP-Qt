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

#ifndef STATISTIC_ANALYZER_CONTROLLER_LABORATORY_CONTROLLER_H_
#define STATISTIC_ANALYZER_CONTROLLER_LABORATORY_CONTROLLER_H_

#include "../Model/laboratory_model.h"

namespace s21 {
    /**
     * @class The LaboratoryController class
     * Контролирует LaboratoryModel class
     * Moveable, non-copyable, final class
     */
    class LaboratoryController final {
    public:
        using statistic_type = LaboratoryStatisticEngine::statistic_type;
        using statistic_value_type = LaboratoryStatisticEngine::statistic_value_type;
        using statistic_size_type = LaboratoryStatisticEngine::statistic_size_type;

    public:
        void readStatisticFromFile(const std::string &path) { model_->readStatisticFromFile(path); }
        void sortStatistic() { model_->sortStatistic(); }
        void buildStatisticIntervals(const statistic_size_type &intervals_count) { model_->buildStatisticIntervals(intervals_count); }

        void showStatistic(std::ostream &out, char sep = '\t') const { model_->showStatistic(out, sep); }
        void showStatisticIntervals(std::ostream &out) const { model_->showStatisticIntervals(out); }

        [[nodiscard]] statistic_value_type getStatisticMeanValue() const { return model_->getStatisticMeanValue(); }
        [[nodiscard]] statistic_value_type getStatisticMeanQuadraticDeviationValue() const { return model_->getStatisticMeanQuadraticDeviationValue(); }
        [[nodiscard]] statistic_value_type getStatisticSum() const { return model_->getStatisticSum(); }
        [[nodiscard]] statistic_size_type  getStatisticSize() const noexcept { return model_->getStatisticSize(); }
        [[nodiscard]] statistic_size_type  getStatisticIntervalsCount() const noexcept { return model_->getStatisticIntervalsCount(); }
        [[nodiscard]] const statistic_type &getStatistic() const noexcept { return model_->getStatistic(); }
        [[nodiscard]] statistic_value_type getStatisticMinimumValue() const noexcept { return model_->getStatisticMinimumValue(); }
        [[nodiscard]] statistic_value_type getStatisticMaximumValue() const noexcept { return model_->getStatisticMaximumValue(); }
        [[nodiscard]] std::pair<statistic_type, statistic_type> getGraphStatisticData() const { return model_->getGraphStatisticData(); }
        [[nodiscard]] std::pair<statistic_type, statistic_type> getGraphNormalDistributionData() const { return model_->getGraphNormalDistributionData(); }

        [[nodiscard]] bool statisticIsGood() const noexcept { return model_->statisticIsGood(); }

        void eraseStatisticValue(const statistic_value_type &value) { model_->eraseStatisticValue(value); }


    private:
        std::unique_ptr<LaboratoryModel> model_{new LaboratoryModel};
    };
}

#endif // !STATISTIC_ANALYZER_CONTROLLER_LABORATORY_CONTROLLER_H_
