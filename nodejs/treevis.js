const util = require('util');

function GetTreeById(nodeId, width, height, cb) {
  const exec = require('child_process').exec;
  var cmd = util.format('%s %s %d %d %d', './../src/treevis.exe', '-n', nodeId, width, height);
  console.log('GetTree: ' + ' cmd ' + cmd);
  exec(cmd, (err, stdout, stderr) => {
  if (err) {
    console.log(stderr);
    cb(stderr);
    return;
  }
  cb(stdout);
  });
}

function GetTreeByIdEnd(nodeIdEnd, width, height, cb) {
  const exec = require('child_process').exec;
  var cmd = util.format('%s %s %d %d %d', './../src/treevis.exe', '-e', nodeIdEnd, width, height);
  console.log('GetTree: ' + ' cmd ' + cmd);
  exec(cmd, (err, stdout, stderr) => {
  if (err) {
    console.log(stderr);
    cb(stderr);
    return;
  }
  cb(stdout);
  });
}

function GetTreeByPoint(x, y, width, height, cb) {
  const exec = require('child_process').exec;
  var cmd = util.format('%s %s %d %d %d %d', './../src/treevis.exe', '-p', x, y, width, height);
  console.log('GetTree: ' + ' cmd ' + cmd);
  exec(cmd, (err, stdout, stderr) => {
  if (err) {
    console.log(stderr);
    cb(stderr);
    return;
  }
  cb(stdout);
  });
}

exports.GetTreeById = GetTreeById;
exports.GetTreeByIdEnd = GetTreeByIdEnd;
exports.GetTreeByPoint = GetTreeByPoint;
