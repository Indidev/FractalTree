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
    connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(save()));

    connect(widthBox, SIGNAL(newValue()), this, SLOT(changedValue()));
    connect(heightBox, SIGNAL(newValue()), this, SLOT(changedValue()));
    connect(branchesBox, SIGNAL(newValue()), this, SLOT(changedValue()));
    connect(depthBox, SIGNAL(newValue()), this, SLOT(changedValue()));
    connect(rootBox, SIGNAL(newValue()), this, SLOT(changedValue()));

    connect(ui->leafSizeSlider, SIGNAL(valueChanged(int)), this, SLOT(changedValue()));

    //connect(ui->leafColor, SIGNAL(clicked()), this, SLOT(clickedLeafColor()));
    connect(ui->treeColor, SIGNAL(clicked()), this, SLOT(clickedTreeColor()));

    connect(timer, SIGNAL(timeout()), this, SLOT(updateTree()));

    connect(&leafColorLCMapper, SIGNAL(mapped(int)), this, SLOT(clickedLeafColor(int)));
    connect(&leafColorRCMapper, SIGNAL(mapped(int)), this, SLOT(rightClickedColor(int)));

    treeColor = Qt::black;
    addLeafColorButton();
    addAddColorButton();

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
                                   treeColor, leafColors);

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
                                       treeColor, leafColors);

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

void FractalTree::clickedLeafColor(int index) {
    changeColor(leafColors[index]);
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
    for (int i = 0; i < leafColorButtons.size(); i++)
        leafColorButtons[i]->setStyleSheet("border: none;background-color: " + colorToRGBA(leafColors[i]) + ";");
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

void FractalTree::addLeafColorButton(QColor color) {
    ExtendedButton *button = new ExtendedButton;
    button->setMinimumHeight(25);
    button->setMaximumHeight(25);
    button->setMinimumWidth(25);
    button->setMaximumWidth(25);

    leafColorLCMapper.setMapping(button, leafColors.size());
    connect(button, SIGNAL(leftClicked()), &leafColorLCMapper, SLOT(map()));

    leafColorRCMapper.setMapping(button, leafColors.size());
    connect(button, SIGNAL(rightClicked()), &leafColorRCMapper, SLOT(map()));

    leafColorButtons.push_back(button);
    leafColors.push_back(color);

    ui->leafColorLayout->addWidget(button);
}

void FractalTree::addAddColorButton() {
    addColorButton = new QPushButton("+");
    addColorButton->setMinimumHeight(25);
    addColorButton->setMaximumHeight(25);
    addColorButton->setMinimumWidth(25);
    addColorButton->setMaximumWidth(25);
    addColorButton->setStyleSheet("font-weight: bold;border:none; background-color: rgba(0, 0, 0, 30);");

    ui->leafColorLayout->addWidget(addColorButton);

    connect(addColorButton, SIGNAL(clicked()), this, SLOT(pushedAddColorButton()));
}

void FractalTree::rightClickedColor(int index) {
    QMenu menu;

    menu.addAction("delete");

    QAction* action = menu.exec(QCursor::pos());
    if (action && action->toolTip() == "delete") {
        deleteColorButton(index);
    }
}

void FractalTree::deleteColorButton(int index) {
    leafColors.removeAt(index);
    leafColorButtons[index]->deleteLater();
    leafColorButtons.removeAt(index);
    changedTree = true;
}

void FractalTree::pushedAddColorButton() {
    ui->leafColorLayout->removeWidget(addColorButton);
    addColorButton->deleteLater();
    addLeafColorButton();
    addAddColorButton();
    updateStyleSheet();
    clickedLeafColor(leafColors.size() - 1);
    changedTree = true;
}
