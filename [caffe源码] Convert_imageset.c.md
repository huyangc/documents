# 写在前面
最近在使用caffe做一些微差图像的分类的工作，使用深度学习的方式，在尝试了AlexNet以及rcnn一系列内容之后，使用caffe进行一些结构的调整，然后在已有结构的基础上，增加一些内容进行图像的分类工作。但是不知道为什么，在同样的配置（solver、train.prototxt）下，使用caffe自带的脚本进行lmdb的生成，然后使用caffe tools进行训练网络就可以正常收敛（用AlexNet在自身数据集上做简单fine tuned），之后参考[fast r-cnn](https://github.com/rbgirshick/fast-rcnn)以及一些github上的[caffe例子](https://github.com/BVLC/caffe/issues?q=python+label%3APython)做自身功能的扩展（主要扩展数据输入层，在数据输入前先进行一些预处理），就会发现神经网络在训练前期会收敛到一个小值，然后突然发散。这个过程感觉很疑惑，即使使用我更改之后的数据输入层，进行和使用caffe tools一样的数据处理操作，也会有如上的现象产生。在查找这个问题的根源的时候，就进行了一些caffe源码的查看和阅读理解，在这里做一个记录和分享，**如果阅读了这系列文章的同学发现了里面的问题，请不吝指出**
# caffe tools方式训练神经网络
*在这里我们使用[AlexNet](https://github.com/BVLC/caffe/tree/master/models/bvlc_alexnet)的训练方式为参考*
## [ImageNet](http://image-net.org/download-images)数据集
### 下载
我们使用ImageNet2010年的数据集进行[AlexNet](https://papers.nips.cc/paper/4824-imagenet-classification-with-deep-convolutional-neural-networks.pdf)的训练，首先去ImageNet官网上找到ILSVRC2010的数据集，注册账号后选择下载。完成下载后即可得到一个完整的图片数据集。
### 制作训练集和测试集
ImageNet官网上应该是存在如何划分这些数据集的方式以及这些数据集所对应的标签的，按照官网上给定的数据集和测试集的划分方式，将整体数据划分成数据集和测试集。*（在caffe的训练模式中，无法使用验证集）*将整体数据划分成数据集合测试集的方法也不是直接挪动图片，只需要生成一个train.txt，以及test.txt即可。  
最终得到的文件结构如下：
 ----------------------
--DATA_ROOT
 ----train.txt
 ----test.txt
 ----image_folder 
 ----------------------
train.txt中的内容如下：
 ------------------------
 image_folder/imagefilename.jpg    label
 ------------------------
label和前面的图片之间的空格是一个\t*（其实只要是空格就行了）* 前面的图片路径是相对于数据集根路径的。
### 使用Tools/convert_imageset 进行lmdb生成
在这里仅使用创建train lmdb的方法进行讲解。下图是convert_imageset的调用方法，需要传入三个参数，即*TRAIN_DATA_DIR *，*TRAIN_DATA_FILE*，*WHERE_TO_CREATE_LMDB*。

![convert_imageset使用方法](http://img.blog.csdn.net/20160616101133743)

其他的一些有--开头的标签是一些控制符，有下图所示的内容：

![convert_imageset控制符](http://img.blog.csdn.net/20160616101648354)

在第一张图片中，我们使用了一个shell脚本去控制调用convert_imageset这个过程，其中的TOOLS指向的就是你所[安装的caffe](http://caffe.berkeleyvision.org/installation.html)的tools的位置。TRAIN_DATA_ROOT就是之前的DATA_ROOT，然后RESIZE_HEIGHT和RESIZE_WIDTH是自身设定的需要将图片缩放的大小。

好了写了这么多，终于到了这篇文章的重点位置了，就是convert_imageset这个工具中具体是怎样生成lmdb这种数据格式的，中间使用的控制符对于数据集都具体做了哪些调整。如果对以上的训练过程还想继续深入了解的话，可以参考[这个](http://caffe.berkeleyvision.org/gathered/examples/imagenet.html)

