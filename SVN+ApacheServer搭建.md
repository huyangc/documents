#挂载磁盘
使用命令mount进行磁盘的挂载，把/dev/sdb挂载到/svn路径上。

#下载安装Subversion
在ubuntu 14.04环境下，使用命令``sudo apt-get install subversion``进行安装，安装完成后主要有``svnadmin``命令，进行仓库的创建 和导入、导出工作。
##仓库创建
``svnadmin create /path/to/xxx``
表示在该目录下创建一个xxx仓库

##仓库导出
``svnadmin dump /path/to/xxx > /path/to/save``
表示将xxx导出到save中

##仓库导入
``svnadmin load /path/to/xxx < /path/to/save``
表示从save中导入到xxx中

#安装Apache Server
使用Apache Server进行用户管理，就可以让所有用户的管理不与单个项目有关。
``sudo apt-get install apache2`` 安装apache2 服务
``sudo apt-get install apache2-utils`` 安装apache2 帮助命令，方便使用。


##配置Apache Server
``sudo a2enmod dav_svn``命令打开dav_svn插件
在文件``/<apache-dir>/mods-enable/dav_svn.conf``中去掉<Location /svn>的注释，同时在这块中加入：
```
  DAV svn
  SVNParentPath  /svn
  AuthType Basic
  AuthName "svn repos"
  AuthUserFile /svn/passwd
  Require valid-user
```
表示使用用户的形式进行访问控制。

##配置用户
使用命令``htpasswd </path/to/passwordfile> <username>``来进行用户的增加。

##重启Apache Server
使用命令``sudo service apache2 restart``进行服务重启。

#更改已有的仓库location
使用TortoiseSVN进行操作比较方便：
步骤：
``右键要更改的已有仓库``->``TortoiseSVN``->``Relocate``->在To URL这一项里填：``http://10.15.82.52/svn/xxx``->``确定``


