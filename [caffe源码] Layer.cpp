# caffe.layer

caffe中的layer.cpp是caffe的基础三类之一*blob, solver, layer* 其中Net是layer的集合，一些layer组合而成构成了Net。

blob中存放的是一个n,k,h,w四维数据，其中还含有一些diff（梯度）等信息。这四维分别代表batch_size, 图片的通道数, 图片的高度, 图片的宽度信息。

solver中定义了整个神经网络的参数定义，包括总体的学习率，动量（用来抑制过拟合）以及学习率下降的方式（经过多少次迭代之后以什么样的一种形式下降），显示周期等。

layer就是caffe中预先定义好的神经网络的层的情况，包括这一层使用怎样的方式进行后先传播等。

## Layer
Layer.cpp是caffe中的所有层的基类，在自定义一个新的Layer的时候，都需要继承这个Layer的基类。在查看fast rcnn的代码的时候*（RoiPoolinglayer)*，发现这个类继承了Layer类，因为想看具体的fast rcnn作者所做的工作，所以这里希望查到哪些是Layer必要的实现的virtual函数，哪些是作者自定义的类。

### 
