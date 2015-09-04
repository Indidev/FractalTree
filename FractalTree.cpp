#include "FractalTree.h"
#include "ui_FractalTree.h"

FractalTree::FractalTree(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FractalTree)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    if (!ui->centralWidget->layout())
        ui->centralWidget->setLayout(new QGridLayout);


    ui->seedEdit->setValidator(new LongValidator);

    curTree = 0;
    leafSize = ui->leafSizeSlider->value();

    render();

    connect(ui->renderButton, SIGNAL(clicked()), this, SLOT(render()));
    connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(save()));
    //connect(ui->leafSizeSlider, SIGNAL(valueChanged(int)), this, SLOT(changeLeafSize()));
    connect(timer, SIGNAL(timeout()), this, SLOT(changeLeafSize()));

    //this->setWindowIcon(QIcon("icon.png"));

    timer->start(100);
}

FractalTree::~FractalTree()
{
    delete timer;
    delete curTree;
    delete ui;
}

void FractalTree::render() {

    delete curTree;

    curTree = new FractalTreeImage(ui->widthBox->value(), ui->heightBox->value(),
                                   ui->branchesBox->value(), ui->depthBox->value(),
                                   ui->rootBox->value(), (float) leafSize / 100.f,
                                   (unsigned int) ui->seedEdit->text().toLong());

    drawTree();

    ui->curSeedEdit->setText(QString::number(curTree->getSeed()));
}

void FractalTree::drawTree() {

    //resize
    QImage resized;
    if (curTree->width() > curTree->height())
        resized = curTree->scaledToHeight(500);
    else
        resized = curTree->scaledToWidth(600);
    //crop rectangle
    QRect rect(resized.width() / 2 - 300, resized.height() / 2 - 250, 600, 500);

    //crop and set
    ui->preview->setPixmap(QPixmap::fromImage(resized.copy(rect)));
}

void FractalTree::changeLeafSize() {
    if (leafSize != ui->leafSizeSlider->value()) {
        leafSize = ui->leafSizeSlider->value();
        delete curTree;
        curTree = new FractalTreeImage(ui->widthBox->value(), ui->heightBox->value(),
                                       ui->branchesBox->value(), ui->depthBox->value(),
                                       ui->rootBox->value(), (float) leafSize / 100.f,
                                       (unsigned int) ui->curSeedEdit->text().toLong());

        drawTree();
    }
}

void FractalTree::save() {
    QString filename = QFileDialog::getSaveFileName(this, "save tree", "", "png (*.png);; xpm(*.xpm);; jpg(*.jpg)");
    if (filename.endsWith("png"))
        curTree->save(filename, "png", Qt::CaseInsensitive);
    else if (filename.endsWith("jpg", Qt::CaseInsensitive) || filename.endsWith("xpm", Qt::CaseInsensitive)) {
        QImage withBg(curTree->width(), curTree->height(), QImage::Format_RGB32);
        withBg.fill(Qt::white);
        QPainter painter(&withBg);
        painter.drawImage(withBg.rect(), *curTree);
        withBg.save(filename);
    }
}
