
#include "domparser.h"

#if QT_VERSION_MAJOR >= 6
#include <QRegularExpression>
#else
#include <QRegExp>
#endif

#include "mainwindow.h"
extern MainWindow* mw_one;
extern bool binPlistFile;

QString strRootType;

DomParser::DomParser() {}
DomParser::~DomParser() {}

DomModel* DomParser::fromDom(QDomDocument d) {
  // get root element
  QDomElement root = d.documentElement();

  // check cosistency

  if (root.tagName() != "plist") {
    qWarning("The file is not a plist!");
    return NULL;
  }

  // get dict element
  QDomElement dict = root.firstChildElement();

  // create model
  DomModel* m = new DomModel();

  // create root item in model
  DomItem* item = m->getRoot()->addChild();
  item->setName("plist");
  item->setType(strRootType);

  // parse doc recursively
  parseElement(dict, item);

  return m;
}

QDomDocument DomParser::toDom(DomModel* m) {
  // create doc with doctype
  QDomDocument doc(
      "plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" "
      "\"http://www.apple.com/DTDs/PropertyList-1.0.dtd\"");

  // create and add processing instruction
  QDomProcessingInstruction instr = doc.createProcessingInstruction(
      "xml", "version=\"1.0\" encoding=\"UTF-8\"");
  doc.appendChild(instr);

  // create root item in doc
  QDomElement doc_root = doc.createElement("plist");

  QDomAttr version = doc.createAttribute("version");
  version.setValue("1.0");

  doc_root.setAttributeNode(version);

  DomItem* item = m->itemForIndex(m->index(0, 0));
  strRootType = item->getType();
  QDomElement doc_dict = doc.createElement(strRootType);

  doc_root.appendChild(doc_dict);

  // add root item to doc
  doc.appendChild(doc_root);

  // get root item from model
  DomItem* model_root = m->getRoot()->child(0);

  // parse model recursively
  parseItem(model_root, doc_dict, doc);

  return doc;
}

void DomParser::parseElement(QDomElement& n, DomItem* item) {
  if (n.hasChildNodes()) {
    QDomElement element = n.firstChildElement();

    int nC = 1;

    while (!element.isNull()) {
      QString tagName = element.tagName();

      if (tagName == "key" || tagName == "array" || tagName == "dict" ||
          tagName == "string" || tagName == "integer" || tagName == "real" ||
          tagName == "data" || tagName == "date" || tagName == "bool") {
        // create child
        DomItem* domItem = item->addChild();

        if (tagName == "key") {
          // get key value
          QString eName = element.firstChild().nodeValue();

          // get val element
          QDomElement val = element.nextSiblingElement();

          // get type and val
          QString eType = val.tagName();

          QString eValue = val.firstChild().nodeValue();

          if (eType == "false") {
            eType = "bool";
            eValue = "false";
          }

          if (eType == "true") {
            eType = "bool";
            eValue = "true";
          }

          if (eType == "data") {
            QByteArray bytes = QByteArray::fromBase64(eValue.toUtf8());
            eValue = ByteToHexStr(bytes);
          }

          /*if (eType == "real") {
            QString a = eValue;
            if (binPlistFile && a.length() > 15) {
              QString a1;
              for (int k = 0; k < a.length(); k++) {
                if (a.mid(k, 1) == "9" && a.mid(k + 1, 1) == "9" &&
                    a.mid(k + 2, 1) == "9" && a.mid(k + 3, 1) == "9" &&
                    a.mid(k + 4, 1) == "9") {
                  a1 = a.mid(0, k);
                  int b = a1.mid(a1.length() - 1, 1).toInt();
                  QString b1 =
                      a1.mid(0, a1.length() - 1) + QString::number(b + 1);
                  a = b1;
                  break;
                }
              }

              for (int k = 0; k < a.length(); k++) {
                if (a.mid(k, 1) == "0" && a.mid(k + 1, 1) == "0") {
                  a1 = a.mid(0, k);
                  a = a1;
                  break;
                }
              }
            }

            eValue = a;
          }*/

          // set data
          domItem->setData(eName, eType, eValue);

          parseElement(val, domItem);

          element = val.nextSiblingElement();
        } else {
          domItem->setType(tagName);

          //新增：数组类型key为空的情况
          QString eName = element.firstChild().nodeValue();
          QString eType = element.tagName();

          QString eValue = element.firstChild().nodeValue();
          if (eType == "data") {
            QByteArray bytes = QByteArray::fromBase64(eValue.toUtf8());
            eValue = ByteToHexStr(bytes);
          }

          domItem->setData(eName, eType, eValue);

          domItem->setName(QString("Item %1").arg(nC++));
          // domItem->setName(QString::number(nC ++));

          parseElement(element, domItem);
          element = element.nextSiblingElement();
        }
      } else {
        qWarning("No value matching the key");

        break;
      }
    }
  }
}

void DomParser::parseItem(DomItem* item, QDomElement& n, QDomDocument& doc) {
  QDomElement element;
  QString name = item->getName();
  QString type = item->getType();
  QString value = item->getValue();

  if (name != "plist") {
#if QT_VERSION_MAJOR >= 6
    QRegularExpression rp("Item\\s\\d+");  //正则表达式，排除Item
#else
    QRegExp rp("Item\\s\\d+");  //正则表达式，排除Item
#endif

    // item without key
#if QT_VERSION_MAJOR >= 6
    QRegularExpressionMatch rpm = rp.match(name);

    if (rpm.hasMatch()) {
#else
    if (rp.exactMatch(name)) {
#endif

      // QDomElement container = doc.createElement(type);

      if (type == "bool") value = value.trimmed();

      if (type == "data") {
#if QT_VERSION_MAJOR >= 6
        QString v = value.remove(QRegularExpression("\\s"));  // 16进制去除所有空格
#else
        QString v = value.remove(QRegExp("\\s"));  // 16进制去除所有空格
#endif

        value = QString::fromLatin1(HexStrToByte(v).toBase64());
      }

      // n.appendChild(container);
      // element = container;

    }  // else {

    QDomElement key;
    QDomText keyText;

    if (name.trimmed().mid(0, 4) == "Item") {
      keyText = doc.createTextNode(value);  //新增：解决数组无键值的情况
    } else {
      key = doc.createElement("key");
      keyText = doc.createTextNode(name);
    }

    if (!key.isNull()) key.appendChild(keyText);
    if (!n.isNull()) n.appendChild(key);

    QDomElement val;
    if (type == "bool")  //新增：bool类型
    {
      if (value.trimmed() == "true")
        val = doc.createElement(QStringLiteral("true"));
      if (value.trimmed() == "false")
        val = doc.createElement(QStringLiteral("false"));
    } else {
      val = doc.createElement(type);
    }

    n.appendChild(val);

    if (type != "array" && type != "dict" && type != "bool") {
      QDomText valText;

      if (type == "data")  //新增：解决16进制字串转换问题
      {
#if QT_VERSION_MAJOR >= 6
        value = value.remove(QRegularExpression("\\s"));  // 16进制去除所有空格
#else
        value = value.remove(QRegExp("\\s"));  // 16进制去除所有空格
#endif

        if (name.trimmed().mid(0, 4) == "Item")
          valText = doc.createTextNode(value);
        else
          valText = doc.createTextNode(
              QString::fromLatin1(HexStrToByte(value).toBase64()));

      } else {
        valText = doc.createTextNode(value);
      }

      // qDebug() << valText.nodeValue();

      val.appendChild(valText);
    }

    element = val;
    //}

  } else
    element = n;

  // get children count
  int c = item->childCount();

  // iterate through children
  for (int i = 0; i < c; ++i) {
    DomItem* child = item->child(i);
    parseItem(child, element, doc);
  }
}

QString DomParser::ByteToHexStr(QByteArray ba) {
  QString str = ba.toHex().toUpper();

  return str;
}

QByteArray DomParser::HexStrToByte(QString value) {
  QByteArray ba;
  QVector<QString> byte;
  int len = value.length();
  int k = 0;
  ba.resize(len / 2);
  for (int i = 0; i < len / 2; i++) {
    // qDebug() << i << k;

    byte.push_back(value.mid(k, 2));
    ba[k / 2] = byte[k / 2].toUInt(nullptr, 16);
    k = k + 2;
  }

  return ba;
}
