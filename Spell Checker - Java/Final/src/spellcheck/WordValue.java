package spellcheck;

public class WordValue 
{
	private String word;
	private int value;
	
	public WordValue(String inword, int invalue)
	{
		word = inword;
		value = invalue;
	}
	
	public String getWord()
	{
		return word;
	}
	
	public int getValue()
	{
		return value;
	}
	
	public String toString()
	{
		return word + value;
	}
	
}
