
#include "fileIndex.h"
#include "ui_fileIndex.h"

fileIndex::fileIndex(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::fileIndex)
{
    ui->setupUi(this);

    ui->picWidget->hide();
    ui->vidWidget->hide();

    picPath.clear();
    vidPath.clear();
    pvPath.clear();

    //设置窗口样式
     setStyleSheet("background:skyblue;");
     font.setBold(true);
    font.setFamily("楷体");

     //设置窗口尺寸
    IMAGE_SIZE.setWidth(238);
    IMAGE_SIZE.setHeight(160);
     ITEM_SIZE.setWidth(238);
    ITEM_SIZE.setHeight(190);
    ui->picWidget->setIconSize(IMAGE_SIZE);
    ui->picWidget->clear();
//    ui->tabWidget->setStyleSheet(

//"QTabBar::tab:selected{background-color:green}"
//"QTabBar::tab:hover{ font-size:20px; color:rgb(55,100,255);padding-top:0px; padding-bottom:0px;}"

// );

          //background: transparent;背景透明
          QString str = (tr("QTabBar::tab{width: %1px;height:46px;font:20px;color:black}").arg(60));
          str.append("QTabWidget::pane{border-width: 0px;border-color:white;border-style:outset;background-color: rgb(132, 171, 208);\
          border-radius: 3px;}");
          //将tab-bar左移动
          str.append("QTabWidget::tab-bar{left:20px;}");
          //点击tab此时的状态
          //margin-left距离左边的tab边缘的距离，margin-right距离右边的tab边缘的距离，color是字体颜色，background-color背景色
          str.append("QTabBar::tab:selected{margin-left: 1;margin-right: 0;color: black;background-color:rbg(100,200,255);} ");
             str.append("QTabBar::tab:!selected{margin-left: 1;margin-right: 0;color: white;background-color:rbg(50,100,255);} ");
             str.append("QTabBar::tab:hover:!selected{color: white;margin-left: 1;margin-right: 0;background-color:rbg(100,200,100);} ");
             //str.append("QTabBar::tab:!selected{margin-top: 10px;margin-bottom: -10px;} ");
             ui->tabWidget->setStyleSheet(str);

     //图片列表基本设置
    ui->picWidget->setResizeMode(QListView::Adjust);
    ui->picWidget->setViewMode(QListView::IconMode);
    ui->picWidget->setMovement(QListView::Static);
    ui->picWidget->setSpacing(8);
    ui->picWidget->clear();
    ui->picWidget->setStyleSheet(
     "QListWidget::Item{padding-top:10px; padding-bottom:4px; }"
     "QListWidget{background:skyblue;}"
     "QListWidget::item:selected{ background:lightred; color:rgb(55,100,255)}"
     "QListWidget::item:selected:!active{border-width:0px;background:lightyellow; font-size:10px; }"
     "QListWidget::Item:hover{font-size:50px; color:rgb(55,100,255);padding-top:0px; padding-bottom:0px;}"
        );

    //去除光标
    ui->picWidget->setFocusPolicy(Qt::NoFocus);
     //去除边框
    ui->picWidget->setFrameShape(QListWidget::NoFrame);

    ui->vidWidget->setIconSize(ITEM_SIZE);
    ui->vidWidget->clear();
    //视频列表基本设置
        ui->vidWidget->setResizeMode(QListView::Adjust);
        ui->vidWidget->setViewMode(QListView::IconMode);
        ui->vidWidget->setMovement(QListView::Static);
        ui->vidWidget->setSpacing(8);
        ui->vidWidget->clear();
        ui->vidWidget->setStyleSheet(
         "QListWidget::Item{padding-top:10px; padding-bottom:4px; }"
         "QListWidget{background:skyblue;}"
         "QListWidget::item:selected{ background:lightred;color:rgb(55,100,255)}"
         "QListWidget::item:selected:!active{border-width:0px;background:lightyellow; }"
         "QListWidget::Item:hover{font-size:50px; color:rgb(55,100,255);padding-top:0px; padding-bottom:0px;}"

        );
        //去除光标
        ui->vidWidget->setFocusPolicy(Qt::NoFocus);
         //去除边框
        ui->vidWidget->setFrameShape(QListWidget::NoFrame);


        connect(ui->picWidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(slot_itemClicked(QListWidgetItem*)));
        connect(ui->vidWidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(slot_itemClicked(QListWidgetItem*)));
       ui->picWidget->setContextMenuPolicy(Qt::CustomContextMenu);
        ui->vidWidget->setContextMenuPolicy(Qt::CustomContextMenu);
}

fileIndex::~fileIndex()
{
    delete ui;
}

//加载显示图片数据
void fileIndex::updatePicData(QString p_name)
{
    //遍历图片目录
     picPath = p_name;
     QDir dir(picPath);
     if(!dir.exists())
     return;
     dir.setFilter(QDir::Files | QDir::NoSymLinks);
     filters << "*.png" <<"*.jpg";
     dir.setNameFilters(filters);
     picList = dir.entryList();
     if(picList.count() <= 0)
     return ;
     //添加显示图片数据
     for(int i=0; i<picList.count(); ++i)
     {
     QPixmap pixmap(picPath + "/" +picList.at(i));
     QListWidgetItem *listWidgetItem = new QListWidgetItem
      (QIcon(pixmap.scaled(IMAGE_SIZE)), picList.at(i));
      listWidgetItem->setFont(QFont(font));
      listWidgetItem->setTextColor(QColor(255,255,255));
      listWidgetItem->setSizeHint(ITEM_SIZE);
      ui->picWidget->insertItem(i, listWidgetItem);
      }
      ui->tabWidget->addTab(ui->picWidget,"图片");

}

//加载显示视频数据
void fileIndex::updataVidData(QString vp_name,QString v_name)
{
    //遍历视频图片目录
     //pvPath = vp_name;
     vidPath = v_name;
     qDebug()<<"vp_name: "<<vp_name;
     QDir v_dir(vidPath);
     if (!v_dir.exists()) {return;}
     // 设置过滤器
     v_dir.setFilter(QDir::Files | QDir::NoSymLinks);
     QStringList v_filters;
     v_filters << "*.avi";; //插入到字符串列表，形成过滤器
     v_dir.setNameFilters(v_filters);
     vidList = v_dir.entryList(); //获取所有文件名


     for(int i=0;i<vidList.count();++i)
          {
              VideoCapture cap((vidPath + "/" +vidList.at(i)).toStdString());
              Mat frame;
              cap.read(frame);
              stringstream str;
              str << (vidList.at(i).split(".").at(0)).toStdString() << ".jpg";
              imwrite("./video/frame/" + str.str(), frame);
              cap.release();
          }

     //添加显示图片数据
     for(int i=0; i<vidList.count(); ++i)
     {
     QString vidFile = vidList.at(i).split(".").at(0)+".avi";


     QPixmap pixmap(vidPath+"/frame/" +vidList.at(i).split(".").at(0)+".jpg");

     QListWidgetItem *listWidgetItem = new QListWidgetItem
     (QIcon(pixmap.scaled(IMAGE_SIZE)), vidFile);
     listWidgetItem->setFont(QFont(font));
     listWidgetItem->setTextColor(QColor(255,255,255));
     listWidgetItem->setSizeHint(ITEM_SIZE);
     ui->vidWidget->insertItem(i, listWidgetItem);
     }
     ui->tabWidget->addTab(ui->vidWidget,"视频");
}

//双击全屏显示或播放
void  fileIndex::slot_itemClicked(QListWidgetItem * item)
{

    QString fileName = item->text();
    //如果是双击图片。
    if(fileName.split(".").at(1)=="jpg"||fileName.split(".").at(1)=="png")
    {
    QSize picSize(1024,600);
    QPixmap pixmap;
    pixmap.load(picPath+"/"+picList.at(ui->picWidget->row(item)));
    QPixmap scaledPixmap= QPixmap(pixmap.scaled(picSize));
    msg = new fullShow(this);
    //将图像传到全屏窗口显示，
    connect(this, SIGNAL(give_message(QPixmap)),msg,SLOT(show_picture(QPixmap)));
    msg->show();
    emit give_message(scaledPixmap);
    this->hide();
    }else if(fileName.split(".").at(1)=="avi"||fileName.split(".").at(1)=="mp4"){
        msg=new fullShow(this);
        connect(this,SIGNAL(give_vedname(QString)),msg,SLOT(show_video(QString)));
        msg->show();
        this->hide();
        emit give_vedname(vidPath+"/"+fileName);
    }
}



void fileIndex::on_exit_bt_clicked()
{
    this->close();
   this->parentWidget()->show();
}


//void fileIndex::contextMenuEvent ( QContextMenuEvent * event )
//{
//    QMenu* popMenu = new QMenu(this);
//        popMenu->addAction(new QAction("删除", this));
//        if(this->itemAt(mapFromGlobal(QCursor::pos())) != NULL) //如果有item则添加"修改"菜单 [1]*
//        {
//            popMenu->addAction(new QAction("修改", this));
//        }

//        popMenu->exec(QCursor::pos()); // 菜单出现的位置为当前鼠标的位置
//}

void fileIndex::on_picWidget_customContextMenuRequested(const QPoint &pos)
{
    QListWidgetItem* curItem = ui->picWidget->itemAt(pos);

    if (curItem == NULL)
        return;

    this->picitem=curItem;
    QMenu *popMenu = new QMenu(this);
    QAction *deleteSeed = new QAction(tr("删除"), this);
    QAction *xiugai = new QAction(tr("修改"), this);
    popMenu->addAction(deleteSeed);
    popMenu->addAction(xiugai);
    connect(deleteSeed, SIGNAL(triggered()), this, SLOT(pic_deleteSeedSlot( )));
    connect(xiugai, SIGNAL(triggered()), this, SLOT(pic_xiugai( )));
    popMenu->exec(QCursor::pos());
    delete popMenu;
    delete deleteSeed;
}
void fileIndex::on_vidWidget_customContextMenuRequested(const QPoint &pos)
{
    QListWidgetItem* curItem = ui->vidWidget->itemAt(pos);

    if (curItem == NULL)
        return;

    this->viditem=curItem;
    QMenu *popMenu = new QMenu(this);
    QAction *deleteSeed = new QAction(tr("删除"), this);
    QAction *xiugai = new QAction(tr("修改"), this);
    popMenu->addAction(deleteSeed);
    popMenu->addAction(xiugai);
    connect(deleteSeed, SIGNAL(triggered()), this, SLOT(vid_deleteSeedSlot( )));
    connect(xiugai, SIGNAL(triggered()), this, SLOT(vid_xiugai( )));
    popMenu->exec(QCursor::pos());
    delete popMenu;
    delete deleteSeed;
}
void fileIndex::pic_deleteSeedSlot(){
    qDebug()<<"delete";
    qDebug()<<this->picitem->text();
    int a=ui->picWidget->currentRow();
    ui->picWidget->takeItem(a);

    QFile::remove("./photo/"+this->picitem->text());//刪除文件

}
void fileIndex::pic_xiugai( ){
     qDebug()<<"xiugai";
     qDebug()<<this->picitem->text();

}
void fileIndex::vid_deleteSeedSlot(){
    qDebug()<<"delete";
    qDebug()<<this->viditem->text();
    int a=ui->vidWidget->currentRow();
    ui->vidWidget->takeItem(a);


    QFile::remove("./video/"+this->viditem->text());//刪除文件
}
void fileIndex::vid_xiugai( ){
     qDebug()<<"xiugai";
     qDebug()<<this->viditem->text();
}



