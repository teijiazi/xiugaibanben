#include <QFile>
#include<QTextStream>
#include<QRegularExpression>
#include<iostream>
using namespace std;

int main(int argc, char *argv[])
{
    if(argc==1){
        cout<<"command\t D:\\qt5142r\\xiugaiBanben.exe"<<endl;
        cout<<"arguments\t %{CurrentProject:FileName}"<<endl;
        cout<<"working directory %{sourceDir}"<<endl;
        return 0;
    }

    QString rcfile;
    rcfile=QString(argv[1]);
    bool isQtPro=rcfile.right(3)=="pro";

    QFile filog(rcfile);
    bool issuc=filog.open(QIODevice::ReadWrite);
    if(!issuc)
    {
        return 1;
    }

    //关联ts
    QTextStream tsnew(&filog);
    tsnew.setCodec(isQtPro?"UTF-8":"UTF-16LE");//必须加，否则为locale=gbk
    QString protxt=tsnew.readAll();
    QString flag=isQtPro?"VERSION = ":"VERSION ";
    QString splt=isQtPro?".":",";
    QRegularExpression reg(flag+"(\\d+).(\\d+).(\\d+).(\\d+)");
    auto match=reg.match(protxt);
    if(match.hasMatch()){
        uint bver=match.captured(4).toUInt()+1;
        protxt.replace(reg,QString(flag+"\\1"+splt+"\\2"+splt+"\\3"+splt+"%1").arg(bver));
    }
    tsnew.seek(0);
    if(!isQtPro){
        tsnew.setGenerateByteOrderMark(true);
    }
    tsnew<<protxt;
    filog.close();
}
