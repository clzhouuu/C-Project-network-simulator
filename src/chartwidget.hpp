#ifndef CHARTWIDGET_H
#define CHARTWIDGET_H

#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

using namespace QtCharts;

class ChartWidget : public QWidget {
  Q_OBJECT

 public:
  ChartWidget(QWidget* parent);
  ~ChartWidget();

  void refreshUtilization(int sum);
  void refreshBar(std::vector<int> sent, int sentSum, std::vector<int> dropped,
                  int droppedSum);
  void reset();

 private:
  QVBoxLayout* mainLayout_; /*!< Main layout for the widget */

  QLineSeries* totUtilSeries_; /*!< Line series for total utilization */
  QChart* totUtilChart_;       /*!< Total utilization chart */
  QValueAxis* totUtilX_;       /*!< Total utilization x-axis */
  QValueAxis* totUtilY_;       /*!< Total utilization y-axis */
  QChartView* totUtilView_;    /*!< Total utilization chart view */

  QBarSet* sentSet_;      /*!< Bar set for the sent packets per node */
  QBarSet* droppedSet_;   /*!< Bar set for the dropped packets per node */
  QBarSeries* barSeries_; /*!< The bar series for the chart */
  QChart* barChart_;      /*!< Bar chart of sent and dropped packets per node */
  QBarCategoryAxis* barX_; /*!< Bar chart x-axis */
  QValueAxis* barY_;       /*!< Bar chart y-axis */
  QChartView* barView_;    /*!< Bar chart view */

  void initializeUI();
};

#endif  // CHARTWIDGET_H