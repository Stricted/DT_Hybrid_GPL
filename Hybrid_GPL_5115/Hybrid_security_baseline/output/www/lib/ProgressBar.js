function ProgressBar(bar) {  
     this.bar = bar;  
     this.speed = 50;  
 }  
   
 ProgressBar.prototype.move = function(moveing, callback) {  
     this.bar.style.width = parseInt(this.bar.style.width) + 1 + "%";   
       
     if(this.bar.style.width == "100%"){   
         window.clearInterval(moveing);  
         if (callback != null) {  
             callback();  
         }  
     }   
   
 }  
   
 ProgressBar.prototype.start = function(callback) {  
     var moveing = window.setInterval(function() {  
         bar.move(moveing, callback);  
     }, this.speed);  
 }  
   
 ProgressBar.prototype.clear = function() {  
     this.bar.style.width = "1%";   
 }  