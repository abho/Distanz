#include "parser.h"
#include <QDebug>

Parser::Parser(Option *option, DBHandler *db, QObject *parent) :
    QObject(parent),mOption(option),mDB(db),mNoDB(false)
{
}



QList<Schiff> Parser::parsFlottenAnsicht(QWebFrame *frame)
{
    qDebug() << Q_FUNC_INFO;
    QList<Schiff> list;

    QWebElement div7 = frame->findAllElements("div[id=\"div7\"]").first();

    if(!div7.isNull()){
        QWebElementCollection div7c = div7.findAll("tr [align=center]");
        if(div7c.count()!=0){
            for (int var = 2; var < div7c.count()-1; ++var) {

                if(div7c.at(var).toInnerXml()!= "Unbekannt"){
                    QWebElementCollection ccc = div7c.at(var).findAll("td");
                    Schiff s;

                    s.name =ccc.at(1).toPlainText();
                    s.planiName = ccc.at(2).toPlainText();
                    s.planiID = ccc.at(2).findAll("a").first().attribute("href").split("=").last();
                    s.gala = ccc.at(3).toPlainText();
                    s.zeit = ccc.at(4).toPlainText();
                    s.owner = "Squadflotte";
                    s.count = ccc.at(6).toPlainText().toInt();
                    s.sendUrl = ccc.at(7).findAll("a").first().attribute("href");
                    s.id = s.sendUrl.split("=").last();

                    if(s.zeit!="Unbekannt"){
                        if(s.zeit.isEmpty()){
                            s.zeit = "Parken";
                        } else {
                            s.zeit = s.zeit.remove(0,5).remove("G").simplified().replace(" ",";");
                            if(s.zeit.contains("v",Qt::CaseInsensitive))
                                s.zeit = "V-Mode";
                        }
                    }
                    // qDebug()<< s.toString();
                    list.append(s);
                }
            }
        }else {
            qDebug() << "nicht gefunden div7c";
        }
    }else {
        qDebug() << "nicht gefunden div7";
    }


    QWebElementCollection c = frame->findAllElements("div [class=\'box fleet-box \']");
    if(c.count()!=0){
        QWebElementCollection cc = c.first().findAll("tr [align=center]");
        for (int var = 2; var < cc.count()-1; ++var) {

            if(cc.at(var).toInnerXml()!= "Unbekannt"){
                QWebElementCollection ccc = cc.at(var).findAll("td");
                Schiff s;

                s.name =ccc.at(1).toPlainText();
                s.planiName = ccc.at(2).toPlainText();
                s.planiID = ccc.at(2).findAll("a").first().attribute("href").split("=").last();
                s.zeit = ccc.at(4).toPlainText();
                s.gala = ccc.at(3).toPlainText();
                s.count = ccc.at(6).toPlainText().toInt();
                s.sendUrl = ccc.at(7).findAll("a").first().attribute("href");
                s.id = s.sendUrl.split("=").last();
                s.owner = ccc.at(5).findFirst("b").toPlainText();
                if(s.zeit!="Unbekannt"){
                    if(s.zeit.isEmpty()){
                        s.zeit = "Parken";
                    } else {
                        s.zeit = s.zeit.remove(0,5).remove("G").simplified().replace(" ",";");
                    }
                }
                // qDebug()<< s.toString();
                if(s.owner != "Squadflotte")
                    list.append(s);
            }
        }
    }

    QWebElementCollection cc =c.at(1).findAll("tr [align=center]").first().findAll("tr [align=center]");

    for (int var = 1; var < cc.count(); ++var) {
        QWebElementCollection ccc = cc.at(var).findAll("td");
        Schiff s;

        s.name =ccc.at(0).toPlainText();
        s.planiName = ccc.at(1).toPlainText();
        s.planiID = ccc.at(1).findAll("a").first().attribute("href").split("=").last();
        s.gala = ccc.at(2).toPlainText();
        s.zeit = "Bergbau";
        s.count = 1;

        s.autoRoute = ccc.at(4).findAll("a").at(1).toPlainText();
        QWebElementCollection cccc = ccc.at(4).findAll("a");

        s.autoRoute = cccc.at(0).toPlainText()+";"+cccc.at(0).attribute("href").split("=").last()+";";
        s.autoRoute.append(cccc.at(1).toPlainText()+";"+cccc.at(1).attribute("href").split("=").last());
        cccc = ccc.at(5).findAll("a");
        if(cccc.count()>0){
            s.sendUrl = ccc.at(5).findAll("a").first().attribute("href");
            s.id = s.sendUrl.split("=").last();
        }
        s.owner ="Abho";


        if(!s.zeit.contains("Fertig")&&!s.zeit.contains("in Flotte")){
            if(s.zeit.isEmpty()){
                s.zeit ="Parken";
            }else {
                s.zeit = s.zeit.remove(0,5).remove("G").simplified().replace(" ",";");
            }
            //qDebug() << s.toString();
            list.append(s);
        }
    }


    cc =c.at(2).findAll("tr [align=center]").first().findAll("tr [align=center]");

    for (int var = 1; var < cc.count(); ++var) {
        QWebElementCollection ccc = cc.at(var).findAll("td");
        Schiff s;

        s.name =ccc.at(0).toPlainText();
        s.planiName = ccc.at(1).toPlainText();
        s.planiID = ccc.at(1).findAll("a").first().attribute("href").split("=").last();
        s.gala = ccc.at(2).toPlainText();
        s.zeit = ccc.at(3).toPlainText();
        s.count = 1;
        QWebElementCollection cccc = ccc.at(5).findAll("a");
        if(cccc.count()>0){
            s.sendUrl = ccc.at(5).findAll("a").first().attribute("href");
            s.id = s.sendUrl.split("=").last();
        }
        s.owner ="Abho";


        if(!s.zeit.contains("Fertig")&&!s.zeit.contains("in Flotte")){
            if(s.zeit.isEmpty()){
                s.zeit ="Parken";
            }else {
                s.zeit = s.zeit.remove(0,5).remove("G").simplified().replace(" ",";");
            }
            //qDebug() << s.toString();
            list.append(s);
        }
    }
    //  qDebug() << Q_FUNC_INFO << "läugtdurch";
    return list;
}

QList<QTreeWidgetItem *> Parser::parsSystemListPage(QWebFrame *frame, QHash<QString, QTreeWidgetItem *> *systeme)
{

    qDebug() << Q_FUNC_INFO;
    QStringList sl;
    sl << "<a href=\"" << "\">" << "</a>" << "-1\">" << "</f" << "Galaxie"<< "<";
    QList<QTreeWidgetItem*> list;

    QTreeWidgetItem *item,*old ;
    QStringList l = frame->toHtml().split("-fleet\" align=\"center\">");
    int i,j;
    QString tmp,ctmp,tmpMid;
    QString name,id,cords;
    int check = 0;
    int gala ;
    QString planis = "";
    if(!mNoDB)
        mDB->startTransaction();
    for (int var = 1; var < l.size(); ++var) {

        ctmp.clear();
        cords.clear();
        tmp = l.at(var);

        i = tmp.indexOf(sl.at(0))+sl.at(0).size();
        j = tmp.indexOf(sl.at(1),i);
        tmpMid = tmp.mid(i,j-i);
        id =tmpMid.section("=",2,2).remove("&amp;galx");
        gala = tmpMid.section("=",3,3).toInt();
        i = tmp.indexOf(sl.at(2),j);
        name = tmp.mid(j+2,i-j-2);

        i = tmp.indexOf(sl.at(3),i)+sl.at(3).size();
        j = tmp.indexOf(sl.at(4),i);
        cords.append(tmp.mid(i,j-i)+"/");
        i = tmp.indexOf(sl.at(3),j)+sl.at(3).size();
        j = tmp.indexOf(sl.at(4),i);
        cords.append(tmp.mid(i,j-i)+"/");
        i = tmp.indexOf(sl.at(3),j)+sl.at(3).size();
        j = tmp.indexOf(sl.at(4),i);
        cords.append(tmp.mid(i,j-i));

        old = systeme->value(cords,NULL);
        if(old == NULL){
            item = new QTreeWidgetItem(QStringList() << name << "0"  << QString::number(check)<< planis);
            item->setData(0,Qt::UserRole+1,id);
            item->setData(0,Qt::UserRole,cords);
            item->setData(0,Qt::UserRole+2,gala);

            if(!mNoDB)
                mDB->insert(item);
            list.append(item);
        } else {
            if( old->text(0) != name){
                old->setText(0,name);
                if(!mNoDB)
                    mDB->updateName(old->data(0,Qt::UserRole).toString(),name);
            }
        }
    }
    if(!mNoDB)
        mDB->stopTransaction();
    return list;
}



QList<ODPlanet> Parser::parsSystemPage(QWebFrame *frame)
{
    QString url = frame->url().toString();
    int i = url.indexOf("sys=")+4;
    int j = url.indexOf("&",i);
    QString sysID = url.mid(i,j-i);
    QString gala = url.mid(j).remove("&galx=");
    QString g("Gr&amp;ouml;&amp;szlig;e: ");
    QString b("Bev&amp;ouml;lkerung: ");
    QString str = frame->toHtml();
    QStringList l = str.split("<table width=\"100\" border=\"0\" cellspacing=\"0\" cellpadding=\"0\">");
    QString exit;
    QList<ODPlanet> mPlaniList;
    if(l.size()>2){
        l.removeFirst();
        QString t = l.takeLast();
        QStringList tList = t.split("</tbody></table></div>");
        if(tList.size()>0){
            l.append(tList.first());

            QString slot,bev,index,name;
            QString tmp;
            QStringList end;
            for (int var = 0; var < l.size(); ++var) {
                i =0;
                j = 0;
                tmp = l.at(var);
                i =tmp.indexOf("index=")+6;
                if((i-6)!= -1){
                    ODPlanet p;
                    j = tmp.indexOf(" ",i)-1;
                    p.index = tmp.mid(i,j-i);
                    i = tmp.indexOf(b,j)+b.size();

                    j = tmp.indexOf("&lt",i);

                    p.bev = tmp.mid(i,j-i).remove('.');

                    i = tmp.indexOf(g)+g.size();
                    j = tmp.indexOf('\'',i);
                    p.slot = tmp.mid(i,j-i);
                    if(p.slot.size()>0)
                        p.slot.chop(1);
                    if( p.slot.size() == 1)
                        p.slot.prepend('0');
                    bool be;
                    qreal r = bev.toDouble(&be);
                    if(be)
                        r = r/1000000.0;
                    else
                        r = 0;
                    p.bev =QString::number(r)+" Mio";

                    end.append("("+slot+"-"+bev+")");
                    qDebug() << j;
                    i = tmp.indexOf(",\'",j)+2;
                    j = tmp.indexOf(":\'",i);
                    qDebug() << i<<j;
                    p.name = tmp.mid(i,j-i).remove("&lt;b&gt;").remove("&lt;/b&gt;");

                    i = tmp.indexOf("\',\'",j)+3;
                    j = tmp.indexOf("\',\'",i);
                    p.owner = tmp.mid(i,j-i);

                    qDebug() << p.index <<p.owner<< p.name << p.bev << p.slot;
                    if(!mNoDB)
                        mDB->insertPlaneten(gala.toInt(),sysID,p.index,p.name,p.owner,p.slot.toInt(),p.bev.remove(".").remove(" Mio").toInt());
                    mPlaniList.append(p);
                }

            }
            end.sort();

            for (int var = end.size()-1; var > -1; --var) {
                exit.append(end.at(var)+",");
            }
            if(exit.size()>0)
                exit.chop(1);
            if(!mNoDB)
                mDB->updatePlanis(sysID,exit);

        }
    }
    return mPlaniList;
}

QList<ODPlanet> Parser::parsOwnPlaniListe(QWebFrame *frame)
{
    QStringList l = frame->toHtml().split("tr style=\"vertical-align:top;text-align:left");
    QList<ODPlanet> list;
    if( l.size()>1){
        l.removeFirst();
        QList<ODPlanet> list;
        for (int var = 0; var < l.size(); ++var) {
            ODPlanet p;
            p.pars(l.at(var));
            list.append(p);
        }
        if(!mNoDB)
            mDB->saveOwnPlanis(list);

    }
    return list;
}

QList<Kampf> Parser::parsPlayerLog(QWebFrame *frame)
{
    QWebElementCollection c = frame->findAllElements("tr[align=center]");

    QList<Kampf> l;
    qDebug() << c.count();
    for (int var = 2; var < c.count(); ++var) {
        QWebElementCollection cTmp = c.at(var).findAll("td");
        if(cTmp.count()>=11){
            if(cTmp.at(8).toPlainText().contains("A:")){

                QString name = cTmp.at(0).toPlainText();
                QStringList list;
                if(!mNoDB)
                    list= mDB->containsPlaniToGala(name,7);
                if(!list.empty()){
                    Kampf k;
                    k.planiName =name;
                    k.cords =list.at(1);
                    k.sysid = list.at(2);
                    k.time = QDateTime::fromString(cTmp.at(1).toPlainText().replace(" ","12;"),"dd.MM.yy;HH:mm:ss").toString("HH:mm:ss dd.MM.yy");
                    k.owner = cTmp.at(6).toPlainText();
                    k.ally = cTmp.at(10).toPlainText();
                    k.berichtUrl = cTmp.at(4).findFirst("a").attribute("href");
                    l.append(k);
                }
            }
        } else {
            qDebug() << "td nicht gefunden " ;
        }
    }

    qSort(l);
    return l;
}

bool Parser::parsSendShipPage(QWebFrame *frame, const QString &zielID, ShipSendTask::SendTyp typ)
{
    qDebug() << Q_FUNC_INFO << zielID;
    QWebElementCollection c = frame->findAllElements("input[name=direktid]");
    if(c.count()>0){
        c.first().setAttribute("value",zielID);
    }else {
        qDebug() << "input[name=direktid] nicht gefunden";
        return false;
    }

    if(typ == ShipSendTask::Direkt){
        c = frame->findAllElements("input[id=\"1\"]");
        if(c.count()>0){
            c.first().evaluateJavaScript("this.click()");
        }else {
            qDebug() << "input[id=\"1\"] nicht gefunden";
            return false;
        }
    }else if(typ == ShipSendTask::VMode){

    }

    c = frame->findAllElements("input[name=Abschicken]");
    if(c.count()>0){
        c.first().evaluateJavaScript("this.click()");
    } else {
        qDebug() << "input[name=Abschicken nicht gefunden";
        return false;
    }
    return true;
}

QStringList Parser::parsPostSendPage(QWebFrame *frame)
{
    QStringList l;
    qDebug() << Q_FUNC_INFO ;
    QWebElement e = frame->findFirstElement("td[align=left]");
    if(e.isNull()){
        qDebug() << "td[align=left] nicht gefunden";
        return l;
    }
    QWebElementCollection c = e.findAll("tr");
    if(c.count()==0){
        qDebug() << "tr nicht gefunden";
        return l;
    }
    QWebElementCollection cc;
    for (int var = 0; var < c.count(); ++var) {
        cc = c.at(var).findAll("td");
        if(cc.count()==0){
            qDebug() << "td nicht gefunden";
            return l;
        }
        QWebElement e= cc.last();
        l << e.toInnerXml().section("<br>",1,1).remove("Ankunft in:").trimmed();
    }
    return l;
}

void Parser::login()
{

}

QTreeWidgetItem *Parser::parsFavPage(QWebFrame *frame)
{
    QTreeWidgetItem *item =new QTreeWidgetItem();

    if(frame->url().toString().contains("orbit")){
        QWebElement e = frame->findFirstElement("td[valign=top]");
        QWebElementCollection c = e.findAll("a");
        QString planiName,owner,ally;
        planiName = c.first().toPlainText();
        if(c.count()>1){
            owner = c.at(1).toPlainText();
            ally = c.at(2).toPlainText();
        }
        QString id = e.findFirst("input").attribute("value");
        item->setData(0,Qt::UserRole,id);
        item->setText(0,"[Orbit] "+ planiName +" "+owner+" "+ally);
    }else {

        QWebElement e = frame->findFirstElement("form[name=gotosys]");
        QString sysUrl;
        QString gala;
        QString sysName;
        if(!e.isNull()){
            QWebElement ee = e.findAll("a").at(1);
            sysUrl = ee.attribute("href").split("=").last();
            gala = ee.toPlainText();
            sysName =  e.findFirst("strong").toPlainText().trimmed();
            qDebug() << gala << sysName << sysUrl ;
            item->setData(0,Qt::UserRole,sysUrl);
            item->setText(0,"[System] "+gala + " "+sysName);
        }

    }
    return item;
}

QString Parser::parsAddPage(QWebFrame *frame)
{
    qDebug() << Q_FUNC_INFO;
    QString url = frame->url().toString();
    QString index;
    if(url.contains("op=orbit")){
        QWebElementCollection c = frame->findAllElements("img");
        foreach (const QWebElement &e, c) {
            if(e.attribute("src").contains("sonne"))
                index = e.parent().attribute("href").split("=").last();
        }
    } else {
        index = url.split("=").last();
    }

    return index;
}

QPair<QTreeWidgetItem*,QString> Parser::parsPlanetenPage(QWebFrame *frame,const QString &id)
{
    qDebug() << Q_FUNC_INFO << frame->url().toString() << id << id.isEmpty();
    QString planiID;
    if(id.isEmpty())
        planiID = frame->url().toString().split("=").last();
    else
        planiID = id;
    QString name = frame->findFirstElement("b").toPlainText();
    QString reTime("returntime = ");
    QString html(frame->toHtml());
    int i = html.indexOf(reTime)+reTime.size();
    QString time = html.mid(i,html.indexOf(";",i)-i);
    QWebElement o = frame->findFirstElement("table[width=\"821\"]").findAll("a").at(4);


    QString cords= o.toPlainText();


    QWebElement e = frame->findFirstElement("table[class=tabletrans]");
    QWebElementCollection cc = e.findAll("b");
    QString wolf =cc.at(2).toPlainText().simplified().remove(".");
    float wolfP = e.findFirst("td[id=wolframproduktion]").toPlainText().remove(".").toFloat()/static_cast<float>(3600);
    QString metal =cc.at(1).toPlainText().simplified().remove(".");
    float metalP = e.findFirst("div[id=metallproduktion]").toPlainText().remove(".").toFloat()/static_cast<float>(3600);
    QString kristal =cc.at(4).toPlainText().simplified().remove(".");
    float kristalP = cc.at(3).toPlainText().simplified().remove(".").toFloat()/static_cast<float>(3600);
    QString erz =cc.at(0).toPlainText().simplified().remove(".");
    float erzP = e.findFirst("td[id=erzproduktion]").toPlainText().remove(".").toFloat()/static_cast<float>(3600);
    QString flur =cc.at(6).toPlainText().simplified().remove(".");
    float flurP = cc.at(5).toPlainText().simplified().remove(".").toFloat()/static_cast<float>(3600);
    QString credits =cc.at(8).toPlainText().simplified().remove(".");
    float ip = cc.at(9).toPlainText().simplified().remove(".").toFloat()/3600.0;
    float creditsP = cc.at(10).toPlainText().simplified().remove(".").split(" ").first().toFloat()/static_cast<float>(3600);




    QString count,bauName;

    if(time != "-1"){
        e = frame->findFirstElement("div[id=returntim]");
        QWebElement p = e.parent().parent().parent();
        count = p.findFirst("font").toPlainText();
        bauName = p.findFirst("a").toPlainText();
    }
    //qDebug() << name <<id<< time << wolf << metal << kristal << erz <<flur <<credits << bauName  << count;
    // qDebug() << wolfP <<metalP << kristalP << erzP << flurP << creditsP;// << erz <<flur <<credits << bauName  << count;
    QString fertigTime,timeLeft;
    fertigTime = time=="-1"?"":QDateTime::currentDateTime().addSecs(time.toInt()).toString("ddd hh:mm:ss");
    timeLeft = time == "-1"?"":makeTime(time.toInt());
    time = time =="-1"?"":time;
    QStringList l;
    l << name <<erz <<metal<<wolf << kristal <<flur <<count+" "+bauName;
    l <<  fertigTime<<timeLeft;
    QTreeWidgetItem *item = new QTreeWidgetItem(l);
    item->setForeground(8,Qt::green);
    item->setData(0,Qt::UserRole,time.toInt());
    item->setData(1,Qt::UserRole,erzP);
    item->setData(1,Qt::UserRole+1,erz.toFloat());
    item->setData(2,Qt::UserRole,metalP);
    item->setData(2,Qt::UserRole+1,metal.toFloat());
    item->setData(3,Qt::UserRole,wolfP);
    item->setData(3,Qt::UserRole+1,wolf.toFloat());
    item->setData(4,Qt::UserRole,kristalP);
    item->setData(4,Qt::UserRole+1,kristal.toFloat());
    item->setData(5,Qt::UserRole,flurP);
    item->setData(5,Qt::UserRole+1,flur.toFloat());
    item->setData(6,Qt::UserRole,ip);
    item->setData(0,Qt::UserRole+1,cords.remove(0,2));
    item->setData(0,Qt::UserRole+2,planiID);
    qDebug() << id;
    return QPair<QTreeWidgetItem*,QString>(item,credits+":"+QString::number(creditsP,'f',3));
}

QList<QTreeWidgetItem*> Parser::parsWerft2Page(QWebFrame *f)
{
    qDebug() << Q_FUNC_INFO;
    QWebElement e = f->findFirstElement("div[class=\'box\']");
    qDebug() << e.isNull();

    QWebElementCollection c = e.findAll("tr[bordercolor=\"#CCCCCC\"]");
    qDebug() << c.count();
    QList<QTreeWidgetItem*> l;
    QString name, erz,metal,wolf,kristall,flur,credits,ip,energie,kolo,angriff,inva,verteidigung,reso,antrieb,lade,hp,flurLade,toxx,scan,kop;
    for (int var = 0; var < c.count(); ++var) {
        name = c.at(var).findFirst("input").attribute("value");

        QWebElementCollection cc = c.at(var).findAll("td[width=\"15%\"]");
        erz = cc.at(0).toPlainText();
        metal =cc.at(2).toPlainText();
        wolf  = cc.at(4).toPlainText();
        kristall = cc.at(6).toPlainText();
        flur = cc.at(8).toPlainText();
        credits = cc.at(10).toPlainText();
        ip = cc.at(12).toPlainText();

        cc = c.at(var).findAll("td[width=\"12%\"]");
        energie = cc.at(0).toPlainText();
        angriff = cc.at(1).toPlainText();
        verteidigung = cc.at(2).toPlainText();
        antrieb = cc.at(3).toPlainText();
        hp = cc.at(4).toPlainText();
        toxx = cc.at(5).toPlainText();
        kop = cc.at(6).toPlainText();

        cc = c.at(var).findAll("td[width=\"20%\"]");
        kolo = cc.at(2).toPlainText();
        inva = cc.at(4).toPlainText();
        reso = cc.at(6).toPlainText();
        lade = cc.at(8).toPlainText();
        flur = cc.at(10).toPlainText();
        scan = cc.at(12).toPlainText();

        QStringList names;
        names << name << Tool::insertDots(metal.remove(".")) << Tool::insertDots(kristall.remove(".")) << Tool::insertDots(wolf.remove("."));
        names << Tool::insertDots(flur.remove(".")) <<  Tool::insertDots(ip.remove(".")) << Tool::insertDots(credits.remove("."))  ;
        QTreeWidgetItem *item = new QTreeWidgetItem(names);
        QString tool;
        tool.append("Energie:"+energie+"\n");
        tool.append("Angriff:"+angriff+"\n");
        tool.append("Verteidigung:"+verteidigung+"\n");
        tool.append("Antrieb:"+antrieb+"\n");
        tool.append("HP:"+hp+"\n");
        tool.append("Toxx::"+toxx+"\n");
        tool.append("KOP:"+kop+"\n");
        tool.append("Kolo:"+kolo+"\n");
        tool.append("Inva:"+inva+"\n");
        tool.append("Reso:"+reso+"\n");
        tool.append("Lade:"+lade+"\n");
        tool.append("FlurLade:"+flurLade+"\n");
        tool.append("Scan:"+scan);
        item->setToolTip(0,tool);
        for (int var = 2; var < 6; ++var) {
            QTreeWidgetItem *newItem = new QTreeWidgetItem();
            newItem->setText(0,QString::number(var)+"x");
            for (int i = 1; i < 7; ++i) {
                newItem->setText(i,Tool::insertDots(QString::number(static_cast<double>(item->text(i).remove(".").remove(",").toInt()*var)*(100.0-static_cast<double>(var))*0.01,'f',0)));
            }
            item->addChild(newItem);

        }
        QTreeWidgetItem *newItem = new QTreeWidgetItem();
        newItem->setText(0,QString::number(10)+"x");
        for (int i = 1; i < 7; ++i) {
            newItem->setText(i,Tool::insertDots(QString::number(static_cast<double>(item->text(i).remove(".").remove(",").toInt()*10)*0.9,'f',0)));
        }
        item->addChild(newItem);
        l.append(item);
    }

    return l;
}

bool Parser::sendShips(QWebFrame *frame,const QList<QString> &ships)
{
    qDebug() << Q_FUNC_INFO;
    for (int var = 0; var < ships.size(); ++var) {
        QWebElement e = frame->findFirstElement("a[href=\"?op=sview&shipid="+ships.at(var)+"\"]").parent().parent().findFirst("input");
        if(e.isNull()){
            qDebug() << "a[href=\"?op=sview&shipid="+ships.at(var)+"\"] nicht gefunden";
            return false;
        }
        e.evaluateJavaScript("this.click()");
    }
    QWebElementCollection e = frame->findAllElements("input[name=Submit]");
    for (int var = 0; var < e.count(); ++var) {
    }
    if(e.count()==0){
        qDebug() << "input[name=Submit] nicht gefunden" ;
        return false;
    }
    e.last().evaluateJavaScript("this.click()");
    return true;
}

void Parser::parsCurrentODDBPage(QWebFrame *frame)
{

    QWebElement e = frame->findFirstElement("table[class=\"data searchtbl\"]");
    QHash<QString,ESystem> system;
    qDebug() << Q_FUNC_INFO << e.isNull();
    QString ally;
    if(!e.isNull()){

        QWebElementCollection c = e.findAll("tr");
        qDebug() << c.count();
        for (int var = 1; var < c.count(); ++var) {
            ESystem sys;
            QWebElementCollection cc= c.at(var).findAll("td");
            if (cc.count()>5){
                if(var==1)
                    ally =cc.at(5).toPlainText();
                sys.gala = cc.at(0).toPlainText().toInt();
                sys.id = cc.at(1).toPlainText();
                sys.planiID =cc.at(2).toPlainText();
                sys.namen.append(cc.at(4).toPlainText());
                sys.ally = ally;

            }else {
                qDebug() << "td nicht gefunden";
            }
            if(system.contains(sys.id)){
                ESystem &s = system[sys.id];
                if(!s.namen.contains(sys.namen.first()))
                    s.namen.append(sys.namen.first());
            }else {
                system.insert(sys.id,sys);
            }
        }
    }else {
        qDebug() << "table[class=\"data searchtbl\"] nicht gefunden" ;
    }
    if(!system.isEmpty()&&!mNoDB)
        mDB->insertEsystemList(system,ally);
}

void Parser::setNoDB(bool noDB)
{
    mNoDB = noDB;
}

QList<QPair<QString,QString> > Parser::parsSystem(QWebFrame *frame)
{
    qDebug() << Q_FUNC_INFO;
    QWebElementCollection c = frame->findAllElements("td[valign=\"top\"]");
    QList<QPair<QString,QString> > l;
    if(c.count()>0){
        for ( int i = 1;i<c.count();i++){
            const QWebElement &e = c.at(i);
            QWebElement ele(e.findFirst("a"));
            if(!ele.isNull()){
                QString str =ele.attribute("onmouseover");
                int i = str.indexOf(";setter('")+9;
                int j = str.indexOf("',",i);
                l.append(QPair<QString,QString>(str.mid(i,j-i),ele.attribute("href").split("=").last()));
            }else {
                qDebug() << "nicht gefunden: a";
            }
        }
    }else {
        qDebug() << "nicht gefunden: td[valign=\"top\"]";
    }
    return l;
}

QList<QPair<QString,QString> > Parser::parsScoutPage(QWebFrame *frame)
{
    QWebElement e = frame->findFirstElement("div[class=\"ajax center\"]");
    QList<QPair<QString,QString> > l;
    if(!e.isNull()){
        QWebElementCollection c= e.findAll("tr");

        if(c.count()>0){
            for (int var = 1; var < c.count(); ++var) {
                QWebElementCollection cc = c.at(var).findAll("td");
                if(cc.count()>1){
                    l.append(QPair<QString,QString>(cc.at(1).toPlainText(),cc.at(2).toPlainText()));
                } else {
                    qDebug() << "td nicht gefunden";
                }
            }
        }else {
            qDebug() << "tr nicht gefunden";
        }

    }else {
        qDebug() << "div[class=\"ajax center\"] nicht egfunden";
    }

    qDebug() << l;
    return  l;
}

QString Parser::makeTime(int t)
{
    int h = t/(3600);
    t=t-h*3600;
    int m = t/60;
    t = t-m*60;
    int s=t;

    QString hh = QString::number(h);
    if(hh.size()==1)
        hh.prepend('0');
    QString mm = QString::number(m);
    if(mm.size()==1)
        mm.prepend('0');
    QString ss = QString::number(s);
    if(ss.size()==1)
        ss.prepend('0');

    return hh+":"+mm+":"+ss;
}
