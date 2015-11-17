package com.example.fangwechat;

import java.util.ArrayList;
import java.util.HashMap;



public class WC_data {

	public static ArrayList<HashMap<String, String>>  get_contactgroudlist() {
		
		   
        ArrayList<HashMap<String, String>> groudlist=new ArrayList<HashMap<String,String>>();
        HashMap<String, String> groud1=new HashMap<String, String>();
        groud1.put("groudname", "A");
        
        HashMap<String, String> groud2=new HashMap<String, String>();
        groud2.put("groudname", "B");
        
        HashMap<String, String> groud3=new HashMap<String, String>();
        groud3.put("groudname", "X");
        
        groudlist.add(groud1);
        groudlist.add(groud2);
        groudlist.add(groud3);
        
		return groudlist;
		
	}
	
	
	
	public static ArrayList<HashMap<String, String>>  get_findgroudlist() {
		
		   
        ArrayList<HashMap<String, String>> groudlist=new ArrayList<HashMap<String,String>>();
        HashMap<String, String> groud1=new HashMap<String, String>();
        groud1.put("groudname", "");
        
        HashMap<String, String> groud2=new HashMap<String, String>();
        groud2.put("groudname", "");
        
        HashMap<String, String> groud3=new HashMap<String, String>();
        groud3.put("groudname", "");
        
        HashMap<String, String> groud4=new HashMap<String, String>();
        groud4.put("groudname", "");
        
     
        
        groudlist.add(groud1);
        groudlist.add(groud2);
        groudlist.add(groud3);
        groudlist.add(groud4);
      
		return groudlist;
		
	}
	
	public static ArrayList<HashMap<String, String>>  get_megroudlist() {
		
		   
        ArrayList<HashMap<String, String>> groudlist=new ArrayList<HashMap<String,String>>();
        HashMap<String, String> groud1=new HashMap<String, String>();
        groud1.put("groudname", "");
        
        HashMap<String, String> groud2=new HashMap<String, String>();
        groud2.put("groudname", "");
        
        HashMap<String, String> groud3=new HashMap<String, String>();
        groud3.put("groudname", "");
        
        HashMap<String, String> groud4=new HashMap<String, String>();
        groud4.put("groudname", "");
        
        HashMap<String, String> groud5=new HashMap<String, String>();
        groud5.put("groudname", "");
        
        groudlist.add(groud1);
        groudlist.add(groud2);
        groudlist.add(groud3);
        groudlist.add(groud4);
        groudlist.add(groud5);
		return groudlist;
		
	}
	
	
	
	public static ArrayList<ArrayList<HashMap<String, Object>>> get_contactChildlist()
	{
		
	      
        HashMap<String, Object> singalHashMap1=new HashMap<String, Object>();
        singalHashMap1.put("character", "郭采");
        singalHashMap1.put("picture", R.drawable.girl1);
        
        HashMap<String, Object> singalHashMap2=new HashMap<String, Object>();
        singalHashMap2.put("character", "留星");
        singalHashMap2.put("picture", R.drawable.girl2);
        
        HashMap<String, Object> singalHashMap3=new HashMap<String, Object>();
        singalHashMap3.put("character", "张飞来");
        singalHashMap3.put("picture", R.drawable.girl3);
        
        HashMap<String, Object> singalHashMap4=new HashMap<String, Object>();
        singalHashMap4.put("character", "和子");
        singalHashMap4.put("picture", R.drawable.girl4);
        
        HashMap<String, Object> singalHashMap5=new HashMap<String, Object>();
        singalHashMap5.put("character", "蓬莱");
        singalHashMap5.put("picture", R.drawable.girl5);
        
      
        
        
        HashMap<String, Object> singalHashMap6=new HashMap<String, Object>();
        singalHashMap6.put("character", "和来");
        singalHashMap6.put("picture", R.drawable.girl2);
        
        
        HashMap<String, Object> singalHashMap7=new HashMap<String, Object>();
        singalHashMap7.put("character", "即客");
        singalHashMap7.put("picture", R.drawable.girl3);
        
        
        HashMap<String, Object> singalHashMap8=new HashMap<String, Object>();
        singalHashMap8.put("character", "飞亚");
        singalHashMap8.put("picture", R.drawable.girl1);
        
        
        HashMap<String, Object> singalHashMap9=new HashMap<String, Object>();
        singalHashMap9.put("character", "蕾蕾");
        singalHashMap9.put("picture", R.drawable.girl4);
        
        
        HashMap<String, Object> singalHashMap10=new HashMap<String, Object>();
        singalHashMap10.put("character", "舅舅");
        singalHashMap10.put("picture", R.drawable.girl1);
        
        HashMap<String, Object> singalHashMap11=new HashMap<String, Object>();
        singalHashMap11.put("character", "妹妹");
        singalHashMap11.put("picture", R.drawable.girl3);
        
        HashMap<String, Object> singalHashMap12=new HashMap<String, Object>();
        singalHashMap12.put("character", "冠子涵");
        singalHashMap12.put("picture", R.drawable.girl2);
        
        
        
        ArrayList<HashMap<String, Object>> list1=new ArrayList<HashMap<String,Object>>();
        list1.add(singalHashMap1);
        list1.add(singalHashMap2);
        list1.add(singalHashMap3);
        
        
        ArrayList<HashMap<String, Object>> list2=new ArrayList<HashMap<String,Object>>();
        list2.add(singalHashMap4);
        list2.add(singalHashMap5);
        list2.add(singalHashMap6);
        list2.add(singalHashMap7);
        list2.add(singalHashMap8);
        list2.add(singalHashMap9);
       
        ArrayList<HashMap<String, Object>> list3=new ArrayList<HashMap<String,Object>>();
        list3.add(singalHashMap10);
        list3.add(singalHashMap11);
        list3.add(singalHashMap12);
//       HashMap<String,  ArrayList<HashMap<String, Object>>> groundHashMap=new HashMap<String, ArrayList<HashMap<String,Object>>>();
//        
//      groundHashMap.put("type1", list1);
//      groundHashMap.put("type2", list2);
        ArrayList<ArrayList<HashMap<String, Object>>> childrenArrayList=new ArrayList<ArrayList<HashMap<String,Object>>>();
        childrenArrayList.add(list1);
        childrenArrayList.add(list2);
        childrenArrayList.add(list3);
        return childrenArrayList;
	}
	
	
	
	
	
	public static ArrayList<ArrayList<HashMap<String, Object>>> get_findChildlist()
	{
		
	      
        HashMap<String, Object> singalHashMap1=new HashMap<String, Object>();
        singalHashMap1.put("character", "朋友圈");
        singalHashMap1.put("picture", R.drawable.friend_circle);
        
        HashMap<String, Object> singalHashMap2=new HashMap<String, Object>();
        singalHashMap2.put("character", "扫一扫");
        singalHashMap2.put("picture", R.drawable.scan);
        
        HashMap<String, Object> singalHashMap3=new HashMap<String, Object>();
        singalHashMap3.put("character", "摇一摇");
        singalHashMap3.put("picture", R.drawable.shockshock);
        
        HashMap<String, Object> singalHashMap4=new HashMap<String, Object>();
        singalHashMap4.put("character", "附近的人");
        singalHashMap4.put("picture", R.drawable.nearme);
        
        HashMap<String, Object> singalHashMap5=new HashMap<String, Object>();
        singalHashMap5.put("character", "漂流瓶");
        singalHashMap5.put("picture", R.drawable.flowbottle);
        
      
        
        
        HashMap<String, Object> singalHashMap6=new HashMap<String, Object>();
        singalHashMap6.put("character", "购物");
        singalHashMap6.put("picture", R.drawable.shopping);
        
        
        HashMap<String, Object> singalHashMap7=new HashMap<String, Object>();
        singalHashMap7.put("character", "游戏");
        singalHashMap7.put("picture", R.drawable.game);
        
        
        HashMap<String, Object> singalHashMap8=new HashMap<String, Object>();
        singalHashMap8.put("character", "飞亚");
        singalHashMap8.put("picture", R.drawable.girl1);
        
        
        HashMap<String, Object> singalHashMap9=new HashMap<String, Object>();
        singalHashMap9.put("character", "蕾蕾");
        singalHashMap9.put("picture", R.drawable.girl4);
        
        
        HashMap<String, Object> singalHashMap10=new HashMap<String, Object>();
        singalHashMap10.put("character", "舅舅");
        singalHashMap10.put("picture", R.drawable.girl1);
        
        HashMap<String, Object> singalHashMap11=new HashMap<String, Object>();
        singalHashMap11.put("character", "妹妹");
        singalHashMap11.put("picture", R.drawable.girl3);
        
        HashMap<String, Object> singalHashMap12=new HashMap<String, Object>();
        singalHashMap12.put("character", "冠子涵");
        singalHashMap12.put("picture", R.drawable.girl2);
        
        
        
        ArrayList<HashMap<String, Object>> list1=new ArrayList<HashMap<String,Object>>();
        list1.add(singalHashMap1);
       
        
        
        ArrayList<HashMap<String, Object>> list2=new ArrayList<HashMap<String,Object>>();
        list2.add(singalHashMap2);
        list2.add(singalHashMap3);
       
       
        ArrayList<HashMap<String, Object>> list3=new ArrayList<HashMap<String,Object>>();
        list3.add(singalHashMap4);
        list3.add(singalHashMap5);
       
        ArrayList<HashMap<String, Object>> list4=new ArrayList<HashMap<String,Object>>();
        list4.add(singalHashMap6);
        list4.add(singalHashMap7);
        
//       HashMap<String,  ArrayList<HashMap<String, Object>>> groundHashMap=new HashMap<String, ArrayList<HashMap<String,Object>>>();
//        
//      groundHashMap.put("type1", list1);
//      groundHashMap.put("type2", list2);
        ArrayList<ArrayList<HashMap<String, Object>>> childrenArrayList=new ArrayList<ArrayList<HashMap<String,Object>>>();
        childrenArrayList.add(list1);
        childrenArrayList.add(list2);
        childrenArrayList.add(list3);
        childrenArrayList.add(list4);
        return childrenArrayList;
	}
	
	
	
	
	
	public static ArrayList<ArrayList<HashMap<String, Object>>> get_meChildlist()
	{
		
	      
        HashMap<String, Object> singalHashMap1=new HashMap<String, Object>();
        singalHashMap1.put("character", "赵晓晓");
        singalHashMap1.put("picture", R.drawable.header);
        
        HashMap<String, Object> singalHashMap2=new HashMap<String, Object>();
        singalHashMap2.put("character", "相册");
        singalHashMap2.put("picture", R.drawable.gallery);
        
        HashMap<String, Object> singalHashMap3=new HashMap<String, Object>();
        singalHashMap3.put("character", "收藏");
        singalHashMap3.put("picture", R.drawable.valuething);
        
        HashMap<String, Object> singalHashMap4=new HashMap<String, Object>();
        singalHashMap4.put("character", "钱包");
        singalHashMap4.put("picture", R.drawable.moneybag);
        
        HashMap<String, Object> singalHashMap5=new HashMap<String, Object>();
        singalHashMap5.put("character", "表情");
        singalHashMap5.put("picture", R.drawable.face);
        
      
        
        
        HashMap<String, Object> singalHashMap6=new HashMap<String, Object>();
        singalHashMap6.put("character", "设置");
        singalHashMap6.put("picture", R.drawable.shopping);
        
        
        HashMap<String, Object> singalHashMap7=new HashMap<String, Object>();
        singalHashMap7.put("character", "游戏");
        singalHashMap7.put("picture", R.drawable.game);
        
        
        HashMap<String, Object> singalHashMap8=new HashMap<String, Object>();
        singalHashMap8.put("character", "飞亚");
        singalHashMap8.put("picture", R.drawable.girl1);
        
        
        HashMap<String, Object> singalHashMap9=new HashMap<String, Object>();
        singalHashMap9.put("character", "蕾蕾");
        singalHashMap9.put("picture", R.drawable.girl4);
        
        
        HashMap<String, Object> singalHashMap10=new HashMap<String, Object>();
        singalHashMap10.put("character", "舅舅");
        singalHashMap10.put("picture", R.drawable.girl1);
        
        HashMap<String, Object> singalHashMap11=new HashMap<String, Object>();
        singalHashMap11.put("character", "妹妹");
        singalHashMap11.put("picture", R.drawable.girl3);
        
        HashMap<String, Object> singalHashMap12=new HashMap<String, Object>();
        singalHashMap12.put("character", "冠子涵");
        singalHashMap12.put("picture", R.drawable.girl2);
        
        
        
        ArrayList<HashMap<String, Object>> list1=new ArrayList<HashMap<String,Object>>();
        list1.add(singalHashMap1);
       
        
        
        ArrayList<HashMap<String, Object>> list2=new ArrayList<HashMap<String,Object>>();
        list2.add(singalHashMap2);
        list2.add(singalHashMap3);
       
       
        ArrayList<HashMap<String, Object>> list3=new ArrayList<HashMap<String,Object>>();
        list3.add(singalHashMap4);
       
       
        ArrayList<HashMap<String, Object>> list4=new ArrayList<HashMap<String,Object>>();
        list4.add(singalHashMap5);
     
        
        ArrayList<HashMap<String, Object>> list5=new ArrayList<HashMap<String,Object>>();
        list5.add(singalHashMap6);
        
       
        
//       HashMap<String,  ArrayList<HashMap<String, Object>>> groundHashMap=new HashMap<String, ArrayList<HashMap<String,Object>>>();
//        
//      groundHashMap.put("type1", list1);
//      groundHashMap.put("type2", list2);
        ArrayList<ArrayList<HashMap<String, Object>>> childrenArrayList=new ArrayList<ArrayList<HashMap<String,Object>>>();
        childrenArrayList.add(list1);
        childrenArrayList.add(list2);
        childrenArrayList.add(list3);
        childrenArrayList.add(list4);
        childrenArrayList.add(list5);
        return childrenArrayList;
	}
	
	
	
	
	
}
