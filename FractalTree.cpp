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

    setWindowIcon(QIcon("ico.png"));


    ui->seedEdit->setValidator(new SeedValidator);

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

    colorDialog.setOption(QColorDialog::ShowAlphaChannel);
    //colorDialog.setParent(this);

    connect(ui->seedEdit, SIGNAL(returnPressed()), this, SLOT(render()));
    connect(ui->renderButton, SIGNAL(clicked()), this, SLOT(render()));
    connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(save()));

    connect(widthBox, SIGNAL(newValue()), this, SLOT(changedValue()));
    connect(heightBox, SIGNAL(newValue()), this, SLOT(changedValue()));
    connect(branchesBox, SIGNAL(newValue()), this, SLOT(changedValue()));
    connect(depthBox, SIGNAL(newValue()), this, SLOT(changedValue()));

    connect(ui->rootWidth, SIGNAL(valueChanged(int)), this, SLOT(changedValue()));
    connect(ui->branchStretch, SIGNAL(valueChanged(int)), this, SLOT(changedValue()));
    connect(ui->leafSizeSlider, SIGNAL(valueChanged(int)), this, SLOT(changedValue()));
    connect(ui->basSize, SIGNAL(valueChanged(int)), this, SLOT(changedValue()));

    //connect(ui->leafColor, SIGNAL(clicked()), this, SLOT(clickedLeafColor()));
    connect(ui->treeColor, SIGNAL(clicked()), this, SLOT(clickedTreeColor()));

    connect(timer, SIGNAL(timeout()), this, SLOT(updateTree()));

    connect(&leafColorLCMapper, SIGNAL(mapped(int)), this, SLOT(clickedLeafColor(int)));
    connect(&leafColorRCMapper, SIGNAL(mapped(int)), this, SLOT(rightClickedColor(int)));

    treeColor = Qt::black;
    addLeafColor();
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

    QString seed = ui->seedEdit->text().trimmed();

    if (seed.startsWith("#")) {
        QStringList seedParts = seed.remove(0, 1).split(":");

        if (!seedParts.empty() && seedParts[0] != "v2") {
            seedParts.insert(7, "100");
            seedParts.insert(7, "100");
        } else {
            seedParts.pop_front();
        }
        if (checkHashList(seedParts)) {
            widthBox->setValue(seedParts[1].toInt());
            heightBox->setValue(seedParts[2].toInt());
            branchesBox->setValue(seedParts[3].toInt());
            depthBox->setValue(seedParts[4].toInt());
            ui->rootWidth->setValue(seedParts[5].toInt());
            ui->leafSizeSlider->setValue(seedParts[6].toInt());
            ui->basSize->setValue(seedParts[7].toInt());
            ui->branchStretch->setValue(seedParts[8].toInt());

            treeColor = getColorFromHash(seedParts[9]);

            ui->leafColorLayout->removeWidget(addColorButton);
            addColorButton->deleteLater();

            leafColors.clear();
            for (QPushButton *button : leafColorButtons) {
                ui->leafColorLayout->removeWidget(button);
                button->deleteLater();
            }
            leafColorButtons.clear();

            for (int i = 10; i < seedParts.size(); i++) {
                addLeafColor(getColorFromHash(seedParts[i]));
            }
            addAddColorButton();

            updateStyleSheet();

            delete curTree;

            curTree = new FractalTreeImage(widthBox->value(), heightBox->value(),
                                           branchesBox->value(), depthBox->value(),
                                           ui->rootWidth->value(),
                                           (float) ui->basSize->value() / 100.f,
                                           (float) ui->branchStretch->value() / 100.f,
                                           (float) ui->leafSizeSlider->value() / 100.f,
                                           (unsigned int) seedParts[0].toLong(),
                                           treeColor, leafColors);
            drawTree();
        } else {
            QErrorMessage em;
            em.showMessage("Hash is not valid");
            em.exec();
        }
    } else {
        delete curTree;
        curTree = new FractalTreeImage(widthBox->value(), heightBox->value(),
                                       branchesBox->value(), depthBox->value(),
                                       ui->rootWidth->value(),
                                       (float) ui->basSize->value() / 100.f,
                                       (float) ui->branchStretch->value() / 100.f,
                                       (float) ui->leafSizeSlider->value() / 100.f,
                                       (unsigned int) ui->seedEdit->text().toLong(),
                                       treeColor, leafColors);
        drawTree();
    }
}

bool FractalTree::checkHashList(QStringList list) {

    if (list.size() < 9) {
        cout << "Hash is to short! " << endl;
        return false;
    }

    //configuration values
    for (int i = 0; i < 9; i++) {
        bool ok;
        list[i].toLongLong(&ok);
        if (!ok) {
            cout << "Hashvalue: " << list[i].toStdString() << " is not valid." << endl;
            return false;
        }
    }

    //color codes
    for (int i = 9; i < list.size(); i++) {
        bool ok;
        ("0x" + list[i]).toLongLong(&ok, 16);
        if (!ok) {
            cout << "Hashvalue: " << list[i].toStdString() << " is not valid." << endl;
            return false;
        }
    }
    return true;
}

QColor FractalTree::getColorFromHash(QString hash) {
    QColor c("#" + hash.left(6));
    c.setAlpha(hash.right(2).toInt(0, 16));
    return c;
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

    //update seed
    ui->curSeedEdit->setText(QString::number(curTree->getSeed()));

    //update hash
    QString hash = "#v2:" + QString::number(curTree->getSeed());
    hash += ":" + QString::number(widthBox->value());
    hash += ":" + QString::number(heightBox->value());
    hash += ":" + QString::number(branchesBox->value());
    hash += ":" + QString::number(depthBox->value());
    hash += ":" + QString::number(ui->rootWidth->value());
    hash += ":" + QString::number(ui->leafSizeSlider->value());
    hash += ":" + QString::number(ui->basSize->value());
    hash += ":" + QString::number(ui->branchStretch->value());
    hash += ":" + treeColor.name().remove(0, 1) + QString::number(treeColor.alpha(), 16);

    for (QColor color : leafColors) {
        hash += ":" + color.name().remove(0, 1) + QString::number(color.alpha(), 16);
    }

    ui->curHashEdit->setText(hash);
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
                                       ui->rootWidth->value(),
                                       (float) ui->basSize->value() / 100.f,
                                       (float) ui->branchStretch->value() / 100.f,
                                       (float) ui->leafSizeSlider->value() / 100.f,
                                       (unsigned int) ui->curSeedEdit->text().toLong(),
                                       treeColor, leafColors);

        drawTree();
    }
}

void FractalTree::save() {
    QSignalMapper sm;
    connect(&sm, SIGNAL(mapped(QObject*)), this, SLOT(changedFilter(QObject*)));

    QFileDialog fd;
    sm.setMapping(&fd, &fd);
    QObject::connect(&fd, SIGNAL(filterSelected(QString)), &sm, SLOT(map()));

    fd.setWindowTitle("Export tree");
    fd.setNameFilter("png (*.png);; xpm(*.xpm);; jpg(*.jpg);; bmp(*.bmp);; ico(*.ico)");
    fd.setAcceptMode(QFileDialog::AcceptSave);
    if (fd.exec()) {
        QString filename = fd.selectedFiles().first();
        QString extension = filename.split(".").last();

        if (!extension.toLower().contains("png|ico|gif|jpg|bmp|xpm")) {
            extension = fd.selectedNameFilter().split(QRegExp("\\("),QString::SkipEmptyParts).first();
            filename += "." + extension;
        }

        cout << extension.toStdString() << endl;
        if (extension.toLower().contains(QRegExp("png|ico|gif")))
            curTree->save(filename);
        else if (filename.toLower().contains(QRegExp("jpg|bmp|xpm"))) {
            QImage withBg(curTree->width(), curTree->height(), QImage::Format_RGB32);
            withBg.fill(Qt::white);
            QPainter painter(&withBg);
            painter.drawImage(withBg.rect(), *curTree);
            withBg.save(filename);
        } else {
            //should do something graphical here...
            cerr << "Extenstion " << extension.toStdString() << " not supported." << endl;
        }
    }
}

void FractalTree::changedFilter(QObject *dialog) {
    QFileDialog *d = (QFileDialog *) dialog;
    d->setDefaultSuffix(d->selectedNameFilter());
    cout << "default suffix: " << d->defaultSuffix().toStdString() << endl;
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

void FractalTree::addLeafColor(QColor color) {
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
        deleteLeafColor(index);
    }
}

void FractalTree::deleteLeafColor(int index) {
    leafColors.removeAt(index);
    leafColorButtons[index]->deleteLater();
    leafColorButtons.removeAt(index);
    changedTree = true;
}

void FractalTree::pushedAddColorButton() {
    ui->leafColorLayout->removeWidget(addColorButton);
    addColorButton->deleteLater();
    addLeafColor();
    addAddColorButton();
    updateStyleSheet();
    clickedLeafColor(leafColors.size() - 1);
    changedTree = true;
}
