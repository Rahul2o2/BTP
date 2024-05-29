function targetDirectory=partialFolderName(partialName,parentDirectory)
    % List all directories in the parent directory
    allDirectories = dir(fullfile(parentDirectory, '*'));
    % Find directories that contain the partial name
    matchingDirectories = allDirectories([allDirectories.isdir]);
    matchingDirectories = matchingDirectories(contains({matchingDirectories.name}, partialName));
    
    if ~isempty(matchingDirectories)
    % Assume the last match for most recently run directory for simplicity
        targetDirectory = fullfile(parentDirectory, matchingDirectories(end).name);
    else
        disp(['No directory found with the partial name: ' partialName]);
    end
end