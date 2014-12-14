package spellcheck;

import java.util.Comparator;

public class WordValueComparator implements Comparator<WordValue>
{

	public int compare(WordValue arg0, WordValue arg1) 
	{
		
		return arg0.getValue() - arg1.getValue();
	}

}
