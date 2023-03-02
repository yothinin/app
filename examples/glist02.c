//
// gcc -o glist02 glist02.c `pkg-config --cflags --libs glib-2.0`
//

#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

const char* provinces[] = {
        "กรุงเทพมหานคร", "สมุทรปราการ", "นนทบุรี", "ปทุมธานี", "พระนครศรีอยุธยา",
        "อ่างทอง", "ลพบุรี", "สิงห์บุรี", "ชัยนาท", "สระบุรี",
        "ชลบุรี", "ระยอง", "จันทบุรี", "ตราด", "ฉะเชิงเทรา",
        "ปราจีนบุรี", "นครนายก", "สระแก้ว", "นครราชสีมา", "บุรีรัมย์",
        "สุรินทร์", "ศรีสะเกษ", "อุบลราชธานี", "ยโสธร", "ชัยภูมิ",
        "อำนาจเจริญ", "หนองบัวลำภู", "ขอนแก่น", "อุดรธานี", "เลย",
        "หนองคาย", "มหาสารคาม", "ร้อยเอ็ด", "กาฬสินธุ์", "สกลนคร",
        "นครพนม", "มุกดาหาร", "เชียงใหม่", "ลำพูน", "ลำปาง",
        "อุตรดิตถ์", "แพร่", "น่าน", "พะเยา", "เชียงราย",
        "แม่ฮ่องสอน", "นครสวรรค์", "อุทัยธานี", "กำแพงเพชร", "ตาก",
        "สุโขทัย", "พิษณุโลก", "พิจิตร", "เพชรบูรณ์", "ราชบุรี",
        "กาญจนบุรี", "สุพรรณบุรี", "นครปฐม", "สมุทรสาคร", "สมุทรสงคราม",
        "เพชรบุรี", "ประจวบคีรีขันธ์", "นครศรีธรรมราช", "กระบี่", "พังงา",
        "ภูเก็ต", "สุราษฎร์ธานี", "ระนอง", "ชุมพร", "สงขลา",
        "สตูล", "ตรัง", "พัทลุง", "ปัตตานี", "ยะลา", "นราธิวาส", "บึงกาฬ"};

gboolean compare_thai_consonants(gunichar a, gunichar b) {
    const gchar *thai_consonants = "กขฃฅฆงจฉชซฌญฎฏฐฑฒณตถทธนบปผฝพฟภมยรฤฤาฦลฬวศษสหฮ";
    if (a == b) {
        return TRUE;
    }
    if (strchr(thai_consonants, a) && strchr(thai_consonants, b)) {
        return a < b;
    }
    return a < b;
}

int main() {
    setlocale(LC_ALL, "en_US.utf8");
    GList *list = NULL;
    
    int max_arr = sizeof (provinces)/sizeof (char*);
    int i;
    for (i = 0; i < max_arr; i++){
      list = g_list_insert_sorted(list, (char*)provinces[i], (GCompareFunc) g_utf8_collate);
    }
    
    GList *iter;
    int j = 0;
    for (iter = list; iter; iter = iter->next) {
      printf("%s, ", (char*) iter->data);
      j++;
      if (j%10 == 0){
        printf ("\b\b");
        printf (" \n");
      }
    }
    printf ("\b\b");
    printf (" ");
    printf ("\nรวมทั้งหมด %d จังหวัด\n", j);

    g_list_free(list);
    return 0;
}
