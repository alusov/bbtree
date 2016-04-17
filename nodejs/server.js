var express = require('express');
var tree = require('./treevis');

var app = express();
app.use(express.static('public'));


app.get('/', function (req, res) {
   res.sendFile( __dirname + '/public/index.htm' );
})

app.get('/node', function (req, res) {
   tree.GetTreeById(req.query.nodeid, req.query.width, req.query.height, function (mystr) {
   res.end(mystr);
   })
})

app.get('/point', function (req, res) {
   tree.GetTreeByPoint(req.query.x, req.query.y, req.query.width, req.query.height, function (mystr) {
   res.end(mystr);
   })
})

var server = app.listen(8081, function () {
  console.log('Server listening at 8081 port.')
})

