#include "chartwidget.hpp"

ChartWidget::ChartWidget(QWidget* parent)
    : QWidget(parent),
      mainLayout_(nullptr),
      totUtilSeries_(nullptr),
      totUtilChart_(nullptr),
      totUtilX_(nullptr),
      totUtilY_(nullptr),
      totUtilView_(nullptr),
      sentSet_(nullptr),
      droppedSet_(nullptr),
      barSeries_(nullptr),
      barChart_(nullptr),
      barX_(nullptr),
      barY_(nullptr),
      barView_(nullptr) {
  initializeUI();
}

ChartWidget::~ChartWidget() {
  delete mainLayout_;
  delete totUtilView_;
  delete barView_;
}

/**
 * @brief Initializes the user interface
 *
 * Sets up all charts with their titles and labels.
 */
void ChartWidget::initializeUI() {
  mainLayout_ = new QVBoxLayout(this);

  totUtilSeries_ = new QLineSeries();
  totUtilChart_ = new QChart();
  totUtilChart_->legend()->hide();
  totUtilChart_->addSeries(totUtilSeries_);
  totUtilX_ = new QValueAxis();
  totUtilX_->setTitleText("Time [ticks]");
  totUtilX_->setLabelFormat("%d");
  totUtilX_->setTickInterval(1);
  totUtilChart_->addAxis(totUtilX_, Qt::AlignBottom);
  totUtilSeries_->attachAxis(totUtilX_);
  totUtilY_ = new QValueAxis();
  totUtilY_->setRange(0, 100);
  totUtilY_->setTickCount(11);
  totUtilY_->setTitleText("Utilization [%]");
  totUtilChart_->addAxis(totUtilY_, Qt::AlignLeft);
  totUtilSeries_->attachAxis(totUtilY_);
  totUtilChart_->setTitle("Total utilization");
  totUtilView_ = new QChartView(totUtilChart_, this);
  mainLayout_->addWidget(totUtilView_);

  sentSet_ = new QBarSet("Sent");
  droppedSet_ = new QBarSet("Dropped");
  barSeries_ = new QBarSeries();
  barSeries_->append(sentSet_);
  barSeries_->append(droppedSet_);
  barChart_ = new QChart();
  barChart_->legend()->setVisible(true);
  barChart_->legend()->setAlignment(Qt::AlignBottom);
  barChart_->addSeries(barSeries_);
  barX_ = new QBarCategoryAxis();
  barX_->setTitleText("Nodes");
  barChart_->addAxis(barX_, Qt::AlignBottom);
  barSeries_->attachAxis(barX_);
  barY_ = new QValueAxis();
  barY_->setTitleText("Amount of packets [%]");
  barY_->setRange(0, 100);
  barChart_->addAxis(barY_, Qt::AlignLeft);
  barSeries_->attachAxis(barY_);
  barChart_->setTitle("Amount of packets sent and dropped");
  barView_ = new QChartView(barChart_, this);
  mainLayout_->addWidget(barView_);
}

/**
 * \brief Updates utilization graph
 *
 * \param sum: Sum of all nodes utilizations
 */
void ChartWidget::refreshUtilization(int sum) {
  int count = totUtilSeries_->count();
  totUtilSeries_->append(count + 1, sum);
  totUtilX_->setRange(0, count + 1);
}

/**
 * \brief Updates bar chart
 *
 * \param sent: Sent packets per node
 * \param sentSum: Sum of all sent packets
 * \param dropped: Dropped packets per node
 * \param droppedSum: Sum of all dropped packets
 */
void ChartWidget::refreshBar(std::vector<int> sent, int sentSum,
                             std::vector<int> dropped, int droppedSum) {
  sentSum = sentSum > 0 ? sentSum : 1;
  droppedSum = droppedSum > 0 ? droppedSum : 1;

  barX_->clear();
  sentSet_->remove(0, sentSet_->count());
  droppedSet_->remove(0, droppedSet_->count());

  for (unsigned int i = 0; i < sent.size(); ++i) {
    barX_->append(QString::number(i));
    sentSet_->append(((float)sent[i] / (float)sentSum) * 100);
    droppedSet_->append(((float)dropped[i] / (float)droppedSum) * 100);
  }
}

void ChartWidget::reset() {
  totUtilSeries_->clear();
  totUtilX_->setRange(0, 1);
  sentSet_->remove(0, sentSet_->count());
  droppedSet_->remove(0, droppedSet_->count());
}
