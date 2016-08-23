function [ varargout ] = loadF(filename)
    fileContent = load(filename, '-ascii');
    d = size(fileContent);
    for i = 1:d(1)
        varargout{i} = fileContent(i,:);
    end
end

