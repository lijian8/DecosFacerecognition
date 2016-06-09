require 'torch'

torch.setdefaulttensortype('torch.FloatTensor')

function hello_world()
  return "Hello World"
end

function tensor_input(data)
  print(data*2)
end

function tensor_output()
  return torch.range(1, 3)
end
