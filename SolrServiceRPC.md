---
title: 使用Motan RPC框架搭建SolrServiceServer
categories: Java
tags: [solr,RPC] 

---
**转载请注明出处**：[blog.justkitt.com](http://blog.justkitt.com)

Motan是新浪开源的一个Java RPC框架，首先RPC是一个很玄乎的概念，但是简单来说，就是字面上的意思即远程过程调用，可以通过RPC框架，调用一个位于网络中的函数，这样就把函数调用这个过程扩充到了很大的范围，同时由于这样的广义上的定义，其实很多通过http协议来获取结果的方式也可以理解成RPC，RPC的概念可以参考[维基百科](https://en.wikipedia.org/wiki/Remote_procedure_call)。

由于RPC的定义，可以很明确地知道RPC是一个C/S模式的技术，即有一部分代码充当服务端，一部分代码充当客户端，然后客户端首先要知道函数的声明情况，这样才能够准确地进行函数的调用，在Java中，通过Interface来进行函数的声明，然后在服务端和客户端两个地方的函数声明应该一致，否则会出现无法调用的问题。然后如果某台服务器固定作为服务端，那么客户端可以直接使用这台服务器的ip进行函数调用，否则需要通过一个第三方来进行服务发现。一般用来做服务发现的第三方为Zookeeper或者是Consul，但是由于目前Solr使用Zookeeper服务器作为一致性保证服务器，所以这里直接使用已有的Zookeeper即可。

具体的配置信息可以参考[Motan的github](https://github.com/weibocom/motan/wiki/zh_userguide)，快速入门可以参考[这里](https://github.com/weibocom/motan/wiki/zh_quickstart)

# 具体配置
由于检索平台的目标是搭建成一个不依靠于任何项目的中间件接口，用于在上层服务和底层数据之间进行连接，所以这里使用RPC的方式，将服务调用进行抽象出来，当前医案系统正需要检索的支持，所以这里用医案系统进行实验。

SolrService的项目SVN地址为：http://10.15.82.52/svn/solrrpcservice

主要包含一个interface声明在客户端和服务器端都使用的函数头。使用Spring配置的客户端和服务器端内容，这里使用Zookeeper作为注册中心，配置文件位于``src\main\resources``路径中，有两个文件，分别表示客户端和服务器端的配置。

客户端和服务器端的代码在``src\main\java\cn\ckcest\zcy``目录下，同时在路径``src\main\java\cn\ckcest\zcy\service\impl``下有一个实现了相应接口的类，这个就是服务服务端提供的"服务"。

## 服务端配置
由于服务端会单独拿出来作为服务，所以需要独立跑在一台服务器上，这里直接使用打包成jar的方式进行。使用maven插件``maven-jar-plugin``和``maven-dependency-plugin``进行打包，具体的配置文件见项目solrrpcservice。打包的相关配置如下：
```
    <build>
        <plugins>
            <plugin>
                <groupId>org.apache.maven.plugins</groupId>
                <artifactId>maven-jar-plugin</artifactId>
                <version>2.6</version>
                <configuration>
                    <archive>
                        <manifest>
                            <addClasspath>true</addClasspath>
                            <classpathPrefix>lib/</classpathPrefix>
                            <mainClass>cn.ckcest.zcy.SolrServiceServer</mainClass>
                        </manifest>
                    </archive>
                </configuration>
            </plugin>
            <plugin>
                <groupId>org.apache.maven.plugins</groupId>
                <artifactId>maven-dependency-plugin</artifactId>
                <version>2.10</version>
                <executions>
                    <execution>
                        <id>copy-dependencies</id>
                        <phase>package</phase>
                        <goals>
                            <goal>copy-dependencies</goal>
                        </goals>
                        <configuration>
                            <outputDirectory>${project.build.directory}/lib</outputDirectory>
                        </configuration>
                    </execution>
                </executions>
            </plugin>
        </plugins>
    </build>
```
之后使用命令mvn clean package即可完成打包，打包完成后，在项目当前路径中多了target文件夹，该文件夹下会有打包出来的jar文件和对应的lib文件夹，lib文件夹下存放该项目依赖的所有lib包，这时候在target路径下使用命令 java -jar xxx.jar即可开启服务。

路径如下所示：
-- target
·   --lib
·   --xx.jar

目前中草药项目组的服务端运行在10.15.82.54上。

## 客户端配置
客户端配置可以参照solrrpcservice进行。这里不再赘述。