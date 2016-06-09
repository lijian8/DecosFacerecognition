require 'torch'
require 'nn'
require 'dpnn'
require 'image'

torch.setdefaulttensortype('torch.FloatTensor')

net = nil
img = torch.Tensor(1, 3, 96, 96)

function load(path)
    net = torch.load(path)
    net:evaluate()
end

function forward_nn(data)
    assert(net, "NeuralNetwork has not been loaded. Run load() first.")
    img[1] = data
    rep = net:forward(img)
    return(rep)
end
