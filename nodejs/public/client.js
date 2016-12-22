var NODE_DISTANCE = 96;
var WINDOW_WIDTH = window.innerWidth;
var WINDOW_HEIGHT = window.innerHeight;
var currentX = 0;
var currentY = 0;
var mouseDownX = 0;
var mouseDownY = 0;

function GetNodes() {
    var nodeid = document.getElementById("node_id").value;   
    var url = "http://127.0.0.1:8081/node?nodeid=" + nodeid + '&width=' + (WINDOW_WIDTH/NODE_DISTANCE) + '&height=' + (WINDOW_HEIGHT/NODE_DISTANCE);
    var xhr = new XMLHttpRequest();
    xhr.open('GET', url, true);  
    xhr.onreadystatechange = function() {   
      if (xhr.readyState == 4 && xhr.status == 200) {        
        var tree = JSON.parse(xhr.responseText);
        if(tree.status !=0){
          alert(tree.error);
          return;
        }                
        var centerNode = tree.Nodes.find(function(node){return node.ID==nodeid;});
        currentX = centerNode.X;
        currentY = centerNode.Y;
        offsetX = (WINDOW_WIDTH / 2) - (centerNode.X * NODE_DISTANCE);
        offsetY = (WINDOW_HEIGHT / 2) - (centerNode.Y * NODE_DISTANCE);      
        DrawTree(tree, offsetX, offsetY);
      }
    };    
    xhr.send(null);
}

function GetNodesFromIdEnd() {
    var nodeid = document.getElementById("node_id_end").value;   
    var url = "http://127.0.0.1:8081/nodeend?nodeid=" + nodeid + '&width=' + (WINDOW_WIDTH/NODE_DISTANCE) + '&height=' + (WINDOW_HEIGHT/NODE_DISTANCE);
    var xhr = new XMLHttpRequest();
    xhr.open('GET', url, true);  
    xhr.onreadystatechange = function() {   
      if (xhr.readyState == 4 && xhr.status == 200) {        
        var tree = JSON.parse(xhr.responseText);
        if(tree.status !=0){
          return;
        }                
        var centerNode = tree.Nodes.find(function(node){return node.IDEnd==nodeid;});
        currentX = centerNode.X;
        currentY = centerNode.Y;
        offsetX = (WINDOW_WIDTH / 2) - (centerNode.X * NODE_DISTANCE);
        offsetY = (WINDOW_HEIGHT / 2) - (centerNode.Y * NODE_DISTANCE);      
        DrawTree(tree, offsetX, offsetY);
      }
    };    
    xhr.send(null);
}


function DrawTree(tree, offsetX, offsetY){
  d3.select("#treesvg").remove();
  var svg = d3.select("body").append("svg")
  .attr("width", WINDOW_WIDTH)
  .attr("height", WINDOW_HEIGHT)
  .attr('id','treesvg')
  .on('mousedown', OnMouseDown)
  .on('mouseup', OnMouseUp);

  DrawLines(tree, offsetX, offsetY);
  DrawCircles(tree, offsetX, offsetY);
  DrawLabels(tree, offsetX, offsetY);					
}

function DrawLines(tree, offsetX, offsetY)
{
  if(tree.Edges != null) {
  d3.select("#treesvg")
  .append('g')
  .attr('id','g_lines')
  .selectAll('line')
  .data(tree.Edges)
  .enter().append('line')
  .attr('x1',function(d){ return d.BeginX * NODE_DISTANCE + offsetX;})
  .attr('y1',function(d){ return d.BeginY * NODE_DISTANCE + offsetY;})
  .attr('x2',function(d){ return d.EndX * NODE_DISTANCE + offsetX;})
  .attr('y2',function(d){ return d.EndY * NODE_DISTANCE + offsetY;});
  }
}

function DrawCircles(tree, offsetX, offsetY)
{
  d3.select("#treesvg")
  .append('g')
  .attr('id','g_circles')
  .selectAll('circle')
  .data(tree.Nodes)
  .enter()
  .append('circle')
  .attr('cx',function(d){ return d.X * NODE_DISTANCE + offsetX;})
  .attr('cy',function(d){ return d.Y * NODE_DISTANCE + offsetY;})
  .attr('r', 8)
  .attr('fill', function(d){ return d.State == 1 ? 'green' : d.State == 2 ? 'red' : 'black';})
  .on('mouseover', OnMouseOver)					
  .on('mouseout', OnMouseOut);
}

function DrawLabels(tree, offsetX, offsetY)
{
  d3.select("#treesvg").
  append('g')
  .attr('id','g_labels')
  .selectAll('text')
  .data(tree.Nodes)
  .enter()
  .append('text')
  .attr('x',function(d){ return d.X * NODE_DISTANCE + offsetX + 12;})
  .attr('y',function(d){ return d.Y * NODE_DISTANCE + offsetY + 5;})
  .text(function(d){ return d.ID;})
  .on('mouseover', OnMouseOver)
  .on('mouseout', OnMouseOut);
}

function OnMouseOver(d)
{
  var div = d3.select("#tool_tip");
  div.transition()		
  .duration(200)	
  .style('opacity', .9);
  div.html(
         'Info: ' + d.Info.trim() + '</br>' +
         '***************************' + '</br>' +
         'ID End: ' + d.IDEnd + '</br>'  +        
         'Level: ' + d.Y + '</br>'  +
         'Parent: ' + d.ParenID + '</br>' +
         'First Child: ' + d.FirstChildId + '</br>' +
         'Left sibling: ' + d.LeftSibling + '</br>' +
         'Right sibling: ' + d.RightSibling + '</br>')
  .style('left', (d3.event.pageX + 20) + 'px')		
  .style('top', (d3.event.pageY - 120) + 'px');
}

function OnMouseOut(d)
{
  var div = d3.select("#tool_tip");
  div.transition()		
  .duration(500)		
  .style('opacity', 0);
}

function OnMouseDown()
{
  var p = d3.mouse(this);
  mouseDownX = p[0];
  mouseDownY = p[1];
}

function OnMouseUp()
{
  var p = d3.mouse(this);
  var offsetX = mouseDownX-p[0];
  var offsetY = mouseDownY-p[1];
  var newCurrentX = currentX + offsetX/NODE_DISTANCE;
  var newCurrentY = currentY + offsetY/NODE_DISTANCE;
  
  var url = "http://127.0.0.1:8081/point?x=" + newCurrentX + '&y=' + newCurrentY + '&width=' + (WINDOW_WIDTH/NODE_DISTANCE) + '&height=' + (WINDOW_HEIGHT/NODE_DISTANCE);
  var xhr = new XMLHttpRequest();
  xhr.open('GET', url, true);  
  xhr.onreadystatechange = function() {     
    if (xhr.readyState == 4 && xhr.status == 200) {
      var tree = JSON.parse(xhr.responseText);
      if(tree.status !=0){
        alert(tree.error);
        return;
      }
      currentX = newCurrentX; 
      currentY = newCurrentY;
      offsetX = (WINDOW_WIDTH / 2) - (currentX * NODE_DISTANCE);
      offsetY = (WINDOW_HEIGHT / 2) - (currentY * NODE_DISTANCE);              
      DrawTree(tree, offsetX, offsetY);
    }
  };    
  xhr.send(null);  
}
