package spellcheck;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.Hashtable;
import java.util.PriorityQueue;
import java.util.Scanner;


public class SpellChecker 
{

	static Hashtable<String, String> dictionary = new Hashtable<String, String>(100000);
	static final char[] letters = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
	static final int[][] letterpos = {{0, 4, 2, 2, 2, 3, 4, 5, 7, 6, 7, 8, 6, 5, 8, 9, 0, 3, 1, 4, 6, 3, 1, 1, 5, 0},{1, 2, 2, 1, 0, 1, 1, 1, 0, 1, 1, 1, 2, 2, 0, 0, 0, 0, 1, 0, 0, 2, 0, 2, 0, 2}};
	static final int[] lettervals = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};
	
	public static void main(String[] args) 
	{
		createDictionary();
		
		Scanner inputscanner = new Scanner(System.in);
		boolean repeat = true;
		do
		{
			ArrayList<String> input = new ArrayList<String>();
			System.out.println("Enter a line to check, or 'exit' to end the program:");
			String s = inputscanner.nextLine();
			s = s.toLowerCase();
			if(s.equals("exit"))
			{
				repeat = false;
				break;
			}
			String[] lineWords = s.split(" ");
			for(String aword : lineWords)
			{
				aword = aword.replaceAll("[^a-zA-Z	]","");
				//System.out.println(aword);
				if(aword.length() > 0)
				{
					input.add(aword);
				}
			}
			
			
			ArrayList<String> corrections = new ArrayList<String>();
			for(String toCheck : input)
			{
				if(dictionary.contains(toCheck))
				{
					//System.out.println(toCheck + " was found.");
				}
				else
				{
					System.out.println(toCheck + " was not found.  Suggestions: ");
					corrections = getSuggestions(toCheck);
					if(corrections.size() == 0)
					{
						System.out.println("No close matches found.");
					}
					else
					{
						for(String corrword : corrections)
						{
							System.out.println(corrword);
						}
					}
				}
				
			}
		} while(repeat);
		
		
		inputscanner.close();
		
		

	}
	
	private static void createDictionary()
	{
		try 
		{
			Scanner dicscanner = new Scanner(new File("dictionary.txt"));
			while(dicscanner.hasNextLine())
			{
				String s = dicscanner.nextLine();
				s = s.toLowerCase();
				String[] lineWords = s.split(" ");
				for(String aword : lineWords)
				{
					aword = aword.replaceAll("[^a-zA-Z	]","");
					//System.out.println(aword);
					dictionary.put(aword, aword);
				}
			}
		dicscanner.close();
		} catch (FileNotFoundException e) 
		{
			e.printStackTrace();
		}
	}
	
	private static ArrayList<String> getSuggestions(String wordcheck)
	{
		ArrayList<String> returnarr = new ArrayList<String>();
		//ArrayList<WordValue> suggs = new ArrayList<WordValue>();
		Comparator<WordValue> comp = new WordValueComparator();
		PriorityQueue<WordValue> wvc = new PriorityQueue<WordValue>(1, comp);
		
		for(int pos = 0; pos <= wordcheck.length(); pos++)
		{
			String a, b, c= null, d= null, e = null, f= null;
			char ca = 0;
			char oldchar = 0;
			if(pos == 0)
			{
				a = "";
				b = wordcheck;
				oldchar = wordcheck.charAt(0);
			}
			else if(pos == wordcheck.length())
			{
				a = wordcheck;
				b = "";
			}
			else
			{
				a = wordcheck.substring(0, pos);
				b = wordcheck.substring(pos);
				c = wordcheck.substring(0, pos);
				d = wordcheck.substring(pos + 1);
				oldchar = wordcheck.charAt(pos);
				e = wordcheck.substring(0, pos - 1);
				f = wordcheck.substring(pos + 1);
				ca = wordcheck.charAt(pos - 1);
				oldchar = wordcheck.charAt(pos);
			}
			for(char ch : letters)
			{
				//Test inserting a letter 
				String test = a + ch + b;
				int distance1 = (int)ch - 97;
				//System.out.println(test);
				if(dictionary.contains(test))
				{
					wvc.add(new WordValue(test, lettervals[distance1]));
				}
				//Test replacing this letter
				if(pos != 0)
				{
					test = c + ch + d;
					if(dictionary.contains(test))
					{
						int distance2 = (int)oldchar - 97;
						int[] chpos = {letterpos[0][distance1], letterpos[1][distance1]};
						int[] ocpos = {letterpos[0][distance2], letterpos[1][distance2]};
						distance1 = (Math.abs(chpos[0] - ocpos[0]) + Math.abs(chpos[1] - ocpos[1]));
						wvc.add(new WordValue(test, distance1));
					}
				}
				
			}
			//Test removing this letter
			if(pos != 0 && pos != wordcheck.length())
			{
				String test = c + d;
				if(dictionary.contains(test))
				{
					wvc.add(new WordValue(test, lettervals[(int)oldchar - 97]));
				}
				//Test switching letters
				test = e + oldchar + ca + f;
				if(dictionary.contains(test))
				{
					wvc.add(new WordValue(test, 2));
				}
			}
			else
			{
				//Test removing first letter
				if (pos == 0)
				{
					String test = wordcheck.substring(1);
					if(dictionary.contains(test))
					{
						wvc.add(new WordValue(test, Math.abs(lettervals[(int)oldchar - 97])));
					}
				}

			}
		}
		
		for(WordValue wv : wvc)
		{
			System.out.print(wv + " ");
		}
		System.out.println();
		
		int i = 0;
		int j = wvc.size();
		while(i < j && i < 7)
		{
			returnarr.add(wvc.remove().getWord());
			i++;
		}
		
		
		return returnarr;
	}
	

}
