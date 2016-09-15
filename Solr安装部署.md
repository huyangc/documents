---
title: Solr安装部署
categories: Solr
tags: [Solr,Zookeeper] 

---

**转载请注明来源：**[blog.justkitt.com](http://blog.justkitt.com)

# 下载安装Solr
[Solr官网](http://archive.apache.org/dist/lucene/solr/)上下载相应版本，在本项目中，使用的版本是Solr-5.1.0。
下载好的是一个压缩版本，使用命令``tar -xvf xxx.tgz``将solr进行解压缩，后文描述的solr的根目录用``<solr-home>``描述

#solr文件结构
```
--solr-home
    --bin    //solr的打开和关闭脚本，主要使用到solr
	--contrib    //一般是solr的第三方库的位置
	--dist		  //一般是solr主要包的位置
	--docs		  //solr的文档
	--example	  //solr的一些例子
	--licenses   //一些license
	--server	  //solr的服务器内容
```
# Solr部署结构
## 线上环境
- Zookeeper服务器：10.15.82.51:2181
- solrnode1：10.15.82.51:8983
- solrnode2：10.15.82.100:8983
- solrnode3：10.15.82.99:8983
- solrnode4：10.15.82.98:8983

其中solrnode2、3、4是在10.15.82.61上的三个虚拟机。内存为22G，硬盘大小为500G，双CPU，6核

## 每个节点的部署方式
在文件路径``/home/zheda/solr/``中有两个文件夹：
```
-solrnode //表示每个solr的collection的内容（实际的索引数据存放的位置）
-resource //表示solr的词典存放的位置。
```
在文件路径``/opt/solr``中存放有之前解压缩得到的实际文件。即``<solr-home>``就是``/opt/solr``

## 分词方法
在本solr中使用[mmseg4j](https://github.com/chenlb/mmseg4j-solr)进行分词，使用其中的max-word分词方式。需要注意的是，分词需要使用[mmseg4j](https://github.com/chenlb/mmseg4j-solr)所提及的两个lib包，将lib包放入``<solr-home>/contrib/analysis-extras``下，共有两个包：``mmseg4j-core-1.10.0.jar, mmseg4j-solr-2.3.0.jar``，定义有以下内容：
```
<fieldType name="text_general" class="solr.TextField" positionIncrementGap="100">
      <analyzer>
        <tokenizer class="com.chenlb.mmseg4j.solr.MMSegTokenizerFactory" mode="max-word" dicPath="/home/zheda/solr/resource/dictionary/vocabulary.dic"/>
        <filter class="solr.StopFilterFactory" ignoreCase="true" words="chinesestopword.dic" />
        <!-- in this example, we will only use synonyms at query time
        <filter class="solr.SynonymFilterFactory" synonyms="index_synonyms.txt" ignoreCase="true" expand="false"/>
        -->
      </analyzer>
</fieldType>
```
其中有fieldType，analyzer和tokenizer三个标签，其中``fieldType``用于标记solr中属性的类型，``analyzer``表示使用何种方式进行分词，然后使用``tokenizer``来进行分词器的定义。

## 服务启动方式
以10.15.82.100上的启动为例，在``<solr-home>``路径下使用命令：
``bin/solr start -cloud -s /home/zheda/solr/solrnode -h 10.15.82.100 -z 10.15.82.51:2181 -p 8983``
命令解析如下：
``-cloud``表示使用SolrCloud模式进行solr的启动
``-h``表示以什么样的主机模式进行启动，一定要制定，不然会变成localhost，对于solrcloud的支持有问题。
``-z``表示Zookeeper服务器的位置是什么。
``-p``表示用什么端口打开solrnode。

打开完成后，Solr会从Zookeeper服务器上根据具体的配置文件进行Core的配置和加载。

## 从Zookeeper服务器下载和上传configuration set
### 下载configuration set
使用如下代码：
```Java
public void exportSynonyms(...) {
    try {
        final CloudSolrClient cloudSolrClient = ... //Retrieve the Solr client
        final String configName = cloudSolrClient.getDefaultCollection();
        final Path configPath = ... //Get or create temporary local file path
        cloudSolrClient.downloadConfig(configName, configPath);
        // Do something
        cloudSolrClient.uploadConfig(configPath, configName);
        // Reload the collection
    } catch (RestClientException | SolrServiceException | IOException var11) {
        this.handleException(var11);
    }
}
```
或者使用solr提供的``<solr-home>/bin/solr``脚本进行下载：
``bin/solr zk downconfig -z 111.222.333.444:2181 -n mynewconfig -d /path/to/configset``

以上内容来自网络中的一个[回答](https://cwiki.apache.org/confluence/display/solr/Using+ZooKeeper+to+Manage+Configuration+Files?focusedCommentId=65147457#comment-65147457)，真心感谢。顺便吐槽一下现在网上搜索一个内容是多么困难。

### 上传configuration set
可以使用在下载那部分的代码进行上传工作，Zookeeper服务器会自动覆盖掉同名文件。对于非同名文件的删除，需要使用Zookeeper的zkCli进行操作。
或者使用solr提供的``<solr-home>/bin/solr``脚本进行上传：
``bin/solr zk upconfig -z 111.222.333.444:2181 -n mynewconfig -d /path/to/configset``

关于``<solr-home>/bin/solr``脚本的更多使用方法，参考[这里](https://cwiki.apache.org/confluence/pages/viewpage.action?pageId=50234737)。

## 设置solr的JVM-Memory
可以在``<solr-home>/bin``目录下的solr.in.sh（linux）里设置``SOLR_JAVA_MEM="-Xms10g -Xmx10g"``表示设置最小堆大小为10g和最大堆大小为10g，这样就不会产生堆扩展而造成的性能下降。

至于当前内容的设置方式，我已经不记得了= =，所以当前在10.15.82.100等服务器上启动时的20G是如何设置的，在我找出结果后再说。之前所说的方式可以达到目标就可以了。


