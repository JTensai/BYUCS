// STUDENT-CORE-BEGIN
// DO NOT EDIT THIS FILE
var catan = catan || {};
catan.definitions = catan.definitions || {};
/*
Provides the input and output elements for communication display.
**/
catan.definitions.DisplayElement.CommunicationDisplay = ( function(){
	
	var OutputElement = function() {
		
 		function OutputElement (id,linesArray,displayLength){
			this.setDisplayElems(linesArray);
			this.setID(id);
			this.displayLength = displayLength;
			this.setView(this.buildView());
			showLast.call(this);
		};
		core.defineProperty(OutputElement.prototype,"ID");
		core.defineProperty(OutputElement.prototype,"Container");
		core.defineProperty(OutputElement.prototype,"DisplayArea");
		core.defineProperty(OutputElement.prototype,"ScrollBox");
		core.defineProperty(OutputElement.prototype,"DisplayLength");
		core.defineProperty(OutputElement.prototype,"DisplayElems");
		core.defineProperty(OutputElement.prototype,"View");
		core.defineProperty(OutputElement.prototype,"OldLength");
		
		OutputElement.prototype.buildView = function(){
			var container = document.createElement("DIV");
				container.setAttribute("id",this.getID());
				
				this.setContainer(container);
			var innerDiv = document.createElement("DIV");
				innerDiv.setAttribute("class","table-box");
				this.setScrollBox(innerDiv);
			var table = document.createElement("TABLE");
				table.setAttribute('class',"table comm-display-table");
				this.setDisplayArea(table);
				container.appendChild(innerDiv);
				innerDiv.appendChild(table);
			
			return container;
		};
		
		var showLast = function(){
			this.getDisplayArea().innerHTML = "";
			var start = 0;
			if (this.getDisplayLength() < 0){
				start = 0;
			}else if (this.getDisplayLength() < this.getDisplayElems().length) {
				start= this.getDisplayElems().length;
			} else {
				start = this.getDisplayElems().length - this.getDisplayLength();
			}
			for (var loopCounter = start; loopCounter < this.getDisplayElems().length; loopCounter++){
				this.getDisplayArea().appendChild(buildLine.call(this, this.getDisplayElems()[loopCounter]));
			}
		};
		var buildLine = function(logLine){
			
			var cell = document.createElement("TD");
				cell.textContent = logLine.message;
			var rowToReturn = document.createElement("TR");
				rowToReturn.className = logLine.className+" log-line";
				rowToReturn.setAttribute("title",logLine.source);
				rowToReturn.appendChild(cell);
			 return rowToReturn;
		};
		
		OutputElement.prototype.setDisplayLength = function(displayLength){
			this.displayLength = displayLength;
			showLast.call(this);
		};
		OutputElement.prototype.getDisplayLength=function(){
			return this.displayLength;
		};
		
		OutputElement.prototype.addLine=function(logLine){
			this.getDisplayElems().push(logLine);
			showLast.call(this);
			
		};
		OutputElement.prototype.setLines=function(logLines){
			this.setDisplayElems(logLines);
			showLast.call(this);
		};
		
		OutputElement.prototype.updateScroll=function(){
			this.getScrollBox().scrollTop = this.getScrollBox().scrollHeight;//to automatically scroll
		};
		
		OutputElement.prototype.getLines=function(){
			return this.getDisplayElems();
		};

		OutputElement.prototype.getDisplayElement=function(){
			return this.getContainer();
		};
		
		return OutputElement;
	}();
	
	//input element...
	var InputElement = (function (){

		var InputElement = function(id,placeHolder,btnText){
			this.setID(id);
			this.setPlaceholderText(placeHolder);
			this.setButtonText(btnText);
			this.setView(this.buildView());
		};
		core.defineProperty(InputElement.prototype,"ID");
		core.defineProperty(InputElement.prototype,"PlaceholderText");
		core.defineProperty(InputElement.prototype,"ButtonText");
		core.defineProperty(InputElement.prototype,"Action");
		core.defineProperty(InputElement.prototype,"TextElem");
		core.defineProperty(InputElement.prototype,"ButtonElem");
		core.defineProperty(InputElement.prototype,"Container");
		core.defineProperty(InputElement.prototype,"Controller");
		core.defineProperty(InputElement.prototype,"View");
		
		InputElement.prototype.buildView = function(){
			var div = document.createElement("div");
				div.setAttribute("id", this.getID());
				div.setAttribute("class", "form-horizontal");
				this.setContainer(div);
			addFilling.call(this);
			
			return div;
		};
		var addFilling = function(){
			if (this.getButtonText() !== undefined){
				var button = document.createElement("button");
					button.setAttribute("class","button-area quarter short chat-button");
					button.textContent = this.getButtonText();
				this.setButtonElem(button);		
				this.getContainer().appendChild(button);
			} 
			var textInput = document.createElement("INPUT");
				textInput.setAttribute('type',"text");
				textInput.setAttribute('placeholder',this.getPlaceholderText());
				this.setTextElem(textInput);
				this.getContainer().appendChild(textInput);
		};
		InputElement.prototype.reset  = function(){
			this.getTextElement().value = "";
		};
		InputElement.prototype.getTextContent = function(){
			return this.getTextElem().value;
		};
		InputElement.prototype.setTextContent = function(textToSet){
			this.getTextElem().value = textToSet;
		};
		InputElement.prototype.getDisplayElement = function(textToSet){
			return this.getContainer();
		};
		InputElement.prototype.hitEnter = function(){
			this.getController().addLine(this.getTextContent());
			this.setTextContent("");
		}
		InputElement.prototype.setHitAction = function(){
			var action = core.makeAnonymousAction(this, this.hitEnter);
			
			this.getButtonElem().onclick = action;
			$(this.getTextElem()).keypress(function(e) {
				if(e.which == 13) {
					action();
				}
			});
		}
		
		return InputElement;
	}());
	
	return {
		OutputElement : OutputElement,
		InputElement : InputElement,
	};
} ());
