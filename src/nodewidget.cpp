#include "nodewidget.hpp"

#include <iostream>

NodeWidget::NodeWidget(QWidget* parent)
    : QWidget(parent),
      charts_(new ChartWidget(this)),
      mainLayout_(new QVBoxLayout(this)),
      textLayout_(new QGridLayout()),
      title_(new QLabel()),
      maxUtilText_(new QLabel()),
      minUtilText_(new QLabel()),
      sentText_(new QLabel()),
      droppedText_(new QLabel()),
      receivedText_(new QLabel()),
      processTimeText_(new QLabel()),
      deliveredMeanText_(new QLabel()),
      deliveredVarianceText_(new QLabel()),
      maxUtil_(new QLabel()),
      minUtil_(new QLabel()),
      sent_(new QLabel()),
      dropped_(new QLabel()),
      received_(new QLabel()),
      processTime_(new QLabel()),
      deliveredMean_(new QLabel()),
      deliveredVariance_(new QLabel()),
      analysis_(new Analysis()) {
  title_->setText("Nodes");
  title_->setObjectName("title");
  title_->setAlignment(Qt::AlignCenter);
  mainLayout_->addWidget(title_);
  mainLayout_->addWidget(charts_);

  initializeText();
}

NodeWidget::~NodeWidget() {
  delete mainLayout_;
  delete title_;
  delete maxUtilText_;
  delete minUtilText_;
  delete sentText_;
  delete droppedText_;
  delete receivedText_;
  delete processTimeText_;
  delete maxUtil_;
  delete minUtil_;
  delete sent_;
  delete dropped_;
  delete received_;
  delete processTime_;
  delete analysis_;
}

/**
 * \brief Initializes the texts of the screen
*/
void NodeWidget::initializeText() {
  maxUtilText_->setText("Max utilization:");
  textLayout_->addWidget(maxUtilText_, 0, 0);
  textLayout_->addWidget(maxUtil_, 0, 1);
  minUtilText_->setText("Min utilization:");
  textLayout_->addWidget(minUtilText_, 1, 0);
  textLayout_->addWidget(minUtil_, 1, 1);
  sentText_->setText("Total sent packets:");
  textLayout_->addWidget(sentText_, 2, 0);
  textLayout_->addWidget(sent_, 2, 1);
  droppedText_->setText("Total dropped packets:");
  textLayout_->addWidget(droppedText_, 3, 0);
  textLayout_->addWidget(dropped_, 3, 1);
  receivedText_->setText("Total received packets:");
  textLayout_->addWidget(receivedText_, 4, 0);
  textLayout_->addWidget(received_, 4, 1);
  processTimeText_->setText("Average packet process time:");
  textLayout_->addWidget(processTimeText_, 5, 0);
  textLayout_->addWidget(processTime_, 5, 1);
  deliveredMeanText_->setText("Average of delivery times:");
  textLayout_->addWidget(deliveredMeanText_, 6, 0);
  textLayout_->addWidget(deliveredMean_, 6, 1);
  deliveredVarianceText_->setText("Variance of delivery times:");
  textLayout_->addWidget(deliveredVarianceText_, 7, 0);
  textLayout_->addWidget(deliveredVariance_, 7, 1);

  mainLayout_->addLayout(textLayout_);
}

/**
 * \brief Updates the GUI with data from graph
 * 
 * \param graph: Graph which to pass to analyse to get data
*/
void NodeWidget::updateValues(Graph* graph) {
  int delivered = analysis_->delivered(*graph);
  int sent = analysis_->sent(*graph);
  int deliveredMean = analysis_->delivery_mean(*graph);
  int deliveredVariance = analysis_->delivery_variance(*graph);
  int processTime = analysis_->speed_mean(*graph);

  std::vector<int> totalUtilization = analysis_->node_utilization_Sp(*graph);
  std::vector<int> droppedPerNode = analysis_->droppedSp(*graph);
  std::vector<int> sentPerNode = analysis_->sentSp(*graph);

  auto it = std::max_element(totalUtilization.begin(), totalUtilization.end());
  maxUtilValue_ = (it == totalUtilization.end()) ? 0 : *it;

  it = std::min_element(totalUtilization.begin(), totalUtilization.end());
  minUtilValue_ = (it == totalUtilization.end()) ? 0 : *it;

  maxUtil_->setText(QString::number(maxUtilValue_));
  minUtil_->setText(QString::number(minUtilValue_));
  received_->setText(QString::number(delivered));
  sent_->setText(QString::number(sent));
  dropped_->setText(QString::number(sent - delivered));
  deliveredMean_->setText(QString::number(deliveredMean));
  deliveredVariance_->setText(QString::number(deliveredVariance));
  processTime_->setText(QString::number(processTime));

  int utilizationSum =
      std::accumulate(totalUtilization.begin(), totalUtilization.end(), 0);
  charts_->refreshUtilization(utilizationSum);

  int sentSum = std::accumulate(sentPerNode.begin(), sentPerNode.end(), 0);
  int droppedSum =
      std::accumulate(droppedPerNode.begin(), droppedPerNode.end(), 0);
  charts_->refreshBar(sentPerNode, sentSum, droppedPerNode, droppedSum);
}

void NodeWidget::reset() {
  maxUtilValue_ = 0;
  minUtilValue_ = 0;
  charts_->reset();
}