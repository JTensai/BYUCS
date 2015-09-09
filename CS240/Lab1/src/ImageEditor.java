
import java.io.*;
import java.util.*;

public class ImageEditor {

	public static void main(String[] args) throws IOException {		
		File srcFile = null;
		File destFile = null;
		String command = null;
		String param = null;
		int blur_length = 0;        
    	for (int i = 0; i < args.length; i++){
    		if (i == 0)
    			srcFile = new File(args[0]);
    		if (i == 1)
    			destFile = new File(args[1]);
    		if (i == 2)
    			command = args[2];
    		if (i == 3){
    			param = args[3];
    	        if (param.length() > 0)
    	        	blur_length = Integer.parseInt(param);
    		}
    	}    	
    	
    	
		Pixel[][] pixelmap = null;
		int count = 0;
		int width = 0;
		int height = 0;
		int max_color_value = 0;
        count = 0;
        int row = 0;
        int col = 0;
        int cur_pixel_num = 0;
        Pixel cur_pixel = null;
        String line = null;
        Scanner scanner = new Scanner(srcFile);
        
        line = scanner.nextLine(); // Gets first line of file
        while (scanner.hasNextInt() == false){
    		scanner.nextLine(); // Skips any comment lines
        }
        
		width = scanner.nextInt();
    	height = scanner.nextInt();
    	pixelmap = new Pixel[height][width];
        max_color_value = scanner.nextInt();
        
        while (scanner.hasNextInt()){      		
    		col = cur_pixel_num % width;
    		row = cur_pixel_num / width;
    		
			cur_pixel = new Pixel();
			pixelmap[row][col] = cur_pixel;
			cur_pixel.setred(scanner.nextInt());
			cur_pixel.setgreen(scanner.nextInt());
			cur_pixel.setblue(scanner.nextInt());
			cur_pixel_num++;
        }
        scanner.close();
        
        
        String out = null;        
        if (command.equals("grayscale")){
        	out = grayscale(pixelmap, width, height);
        }        
        else if (command.equals("invert")){
        	out = invert(pixelmap, width, height);
        }        
        else if (command.equals("emboss")){
        	emboss(pixelmap, width, height);
        	out = print_pixel_map(pixelmap, width, height);
        }        
        else if (command.equals("motionblur")){
        	out = motionblur(pixelmap, width, height, blur_length);
        }

        FileWriter writer = new FileWriter(destFile);        
        writer.write(line + "\n");
        writer.write(width + " " + height + "\n");
        writer.write(max_color_value + "\n");
        writer.write(out);        
        writer.close();
        
//        System.out.println("Done");
    }


	private static String invert(Pixel[][] pixelmap, int width, int height){
		StringBuilder builder = new StringBuilder();
    	for(int row = 0; row < height; row++){
    		for(int col = 0; col < width; col++){
        		pixelmap[row][col].invert();
        		builder.append(pixelmap[row][col].toString());
        	}
        }
        return builder.toString();		
    }    
    
    private static String grayscale(Pixel[][] pixelmap, int width, int height){
		StringBuilder builder = new StringBuilder();
    	for(int row = 0; row < height; row++){
    		for(int col = 0; col < width; col++){
        		pixelmap[row][col].gray();
        		builder.append(pixelmap[row][col].toString());
        	}
        }
        return builder.toString();	
    }
    
    private static void emboss(Pixel[][] pixelmap, int width, int height){
		for(int row = height-1; row >= 0; row--){
			for(int col = width-1; col >= 0; col--){
				if (col-1 < 0 || row-1 < 0)
					pixelmap[row][col].emboss(pixelmap[row][col]);
				else
					pixelmap[row][col].emboss(pixelmap[row-1][col-1]);
        	}
        }	
    }
    
    private static String motionblur(Pixel[][] pixelmap, int width, int height, int blur_length){
		StringBuilder builder = new StringBuilder();
    	if (blur_length > 0){
        	for(int row = 0; row < height; row++){
        		for(int col = 0; col < width; col++){
	//        		System.out.println(col + " col " + row);
	            	int total_red = 0;
	            	int total_green = 0;
	            	int total_blue = 0;
	            	int divide_length = 0;
	            	for (int i = 0; i < blur_length; i++){
	            		if ((col + i) < width){
	                		total_red += pixelmap[row][col+i].red();
	                		total_green += pixelmap[row][col+i].green();
	                		total_blue += pixelmap[row][col+i].blue();   
	                		divide_length++;
	            		}
	            	}
	            	pixelmap[row][col].setred(total_red/(divide_length));
	            	pixelmap[row][col].setgreen(total_green/(divide_length));
	            	pixelmap[row][col].setblue(total_blue/(divide_length));

	        		builder.append(pixelmap[row][col].toString());
	        	}
	        }
    	}
        return builder.toString();	
    }
    	
	private static String print_pixel_map(Pixel[][] pixelmap, int width, int height) throws IOException {
		StringBuilder builder = new StringBuilder();
		for(int row = 0; row < height; row++){
			for(int col = 0; col < width; col++){
        		builder.append(pixelmap[row][col].toString());
        	}
        }        
        String out = builder.toString();
        return out;		
	}
}

class Pixel {
    int red;
    int green;
    int blue;

    public Pixel() {
        red = 0;
        green = 0;
        blue = 0;
    }

    public int red() {
        return red;
    }
    public void setred(int new_red) {
    	red = new_red;
    }

    public int green() {
        return green;
    }
    public void setgreen(int new_green) {
    	green = new_green;
    }

    public int blue() {
        return blue;
    }
    public void setblue(int new_blue) {
    	blue = new_blue;
    }
    
    public void gray(){
		int average = (red + green + blue)/3;
		setred(average);
		setgreen(average);
		setblue(average);
    }
    public void invert(){
    	setred(255 - red);
    	setgreen(255 - green);
    	setblue(255 - blue);
    }
    public void emboss(Pixel topleft){		
		int redDiff = red() - topleft.red();
		int greenDiff = green() - topleft.green();
		int blueDiff = blue() - topleft.blue();
		int maxDiff = blueDiff;
				
		if (Math.abs(greenDiff) >= Math.abs(blueDiff))
			maxDiff = greenDiff;
		if ((Math.abs(redDiff) >= Math.abs(greenDiff)) && (Math.abs(redDiff) >= Math.abs(blueDiff)) )
			maxDiff = redDiff;
		
		maxDiff = maxDiff + 128;
		
		if (maxDiff < 0)
			maxDiff = 0;
		else if (maxDiff > 255)
			maxDiff = 255;
		    	
		setred(maxDiff);
		setgreen(maxDiff);
		setblue(maxDiff);
    }
    public String toString(){
    	return (red + " " + green + " " + blue + "\n");
    }
}









