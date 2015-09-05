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
    widthBox = new SpinBox;
    widthBox->setMinimum(10);
    widthBox->setMaximum(100000);
    widthBox->setValue(1920);
    ui->optionsForm->setWidget(2, QFormLayout::FieldRole, widthBox);

    heightBox = new SpinBox;
    heightBox->setMinimum(10);
    heightBox->setMaximum(100000);
    heightBox->setValue(1080);
    ui->optionsForm->setWidget(3, QFormLayout::FieldRole, heightBox);

    branchesBox = new SpinBox;
    branchesBox->setMinimum(0);
    branchesBox->setMaximum(100);
    branchesBox->setValue(2);
    ui->optionsForm->setWidget(4, QFormLayout::FieldRole, branchesBox);

    depthBox = new SpinBox;
    depthBox->setMinimum(0);
    depthBox->setMaximum(100);
    depthBox->setValue(10);
    ui->optionsForm->setWidget(5, QFormLayout::FieldRole, depthBox);

    rootBox = new SpinBox;
    rootBox->setMinimum(1);
    rootBox->setMaximum(10000);
    rootBox->setValue(10);
    ui->optionsForm->setWidget(6, QFormLayout::FieldRole, rootBox);

    colorDialog.setOption(QColorDialog::ShowAlphaChannel);
    //colorDialog.setParent(this);

    connect(ui->renderButton, SIGNAL(clicked()), this, SLOT(render()));

    connect(widthBox, SIGNAL(newValue()), this, SLOT(changedValue()));
    connect(heightBox, SIGNAL(newValue()), this, SLOT(changedValue()));
    connect(branchesBox, SIGNAL(newValue()), this, SLOT(changedValue()));
    connect(depthBox, SIGNAL(newValue()), this, SLOT(changedValue()));
    connect(rootBox, SIGNAL(newValue()), this, SLOT(changedValue()));

    connect(ui->leafSizeSlider, SIGNAL(valueChanged(int)), this, SLOT(changedValue()));

    connect(ui->leafColor, SIGNAL(clicked()), this, SLOT(clickedLeafColor()));
    connect(ui->treeColor, SIGNAL(clicked()), this, SLOT(clickedTreeColor()));

    connect(timer, SIGNAL(timeout()), this, SLOT(updateTree()));

    treeColor = Qt::black;
    leafColor = QColor(0, 198, 0, 200);

    updateStyleSheet();
    changedTree = false;

    timer->start(100);
    render();
}

FractalTree::~FractalTree()
{
    delete timer;
    delete curTree;
    delete ui;
}

void FractalTree::render() {

    delete curTree;

    curTree = new FractalTreeImage(widthBox->value(), heightBox->value(),
                                   branchesBox->value(), depthBox->value(),
                                   rootBox->value(), (float) ui->leafSizeSlider->value() / 100.f,
                                   (unsigned int) ui->seedEdit->text().toLong(),
                                   treeColor, leafColor);

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

void FractalTree::changedValue() {
    changedTree = true;
}

void FractalTree::updateTree() {
    if (changedTree) {
        changedTree = false;
        delete curTree;
        curTree = new FractalTreeImage(widthBox->value(), heightBox->value(),
                                       branchesBox->value(), depthBox->value(),
                                       rootBox->value(), (float) ui->leafSizeSlider->value() / 100.f,
                                       (unsigned int) ui->curSeedEdit->text().toLong(),
                                       treeColor, leafColor);

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

void FractalTree::clickedLeafColor() {
    changeColor(leafColor);
    updateStyleSheet();
    changedTree = true;
}

void FractalTree::clickedTreeColor() {
    changeColor(treeColor);
    updateStyleSheet();
    changedTree = true;
}

void FractalTree::changeColor(QColor &curColor) {
    QTimer colorTimer;
    colorPtr = &curColor;
    QColor oldC = curColor;

    colorDialog.setCurrentColor(oldC);

    connect(&colorTimer, SIGNAL(timeout()), this, SLOT(updateColor()));

    //timer for live preview of color
    colorTimer.start(100);

    if (colorDialog.exec()) {
        colorTimer.stop();
        curColor = colorDialog.currentColor();
    } else {
        colorTimer.stop();
        curColor = oldC;
    }

}

void FractalTree::updateStyleSheet() {
    ui->leafColor->setStyleSheet("border: none;background-color: " + colorToRGBA(leafColor) + ";");
    ui->treeColor->setStyleSheet("border: none;background-color: " + colorToRGBA(treeColor) + ";");
}

QString FractalTree::colorToRGBA(const QColor &color) {
    QString style = "rgba( ";
    style += QString::number(color.red()) + ", ";
    style += QString::number(color.green()) + ", ";
    style += QString::number(color.blue()) + ", ";
    style += QString::number(color.alpha()) + ")";
    return style;
}

void FractalTree::updateColor() {
    *colorPtr = colorDialog.currentColor();
    changedTree = true;
}
