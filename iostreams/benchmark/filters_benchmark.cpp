/// @file
/// @brief
/// @copyright Copyright (c) InfoTeCS. All Rights Reserved.

#include <sstream>
#include <iterator>

#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/null.hpp>
#include <boost/iostreams/filtering_stream.hpp>

#include <celero/Celero.h>

#include <iostreams/filters.h>


using namespace std::literals;


namespace {
namespace bm_env {
namespace consts {


static constexpr auto N_SAMPLES = 3u;
static constexpr auto N_ITERATIONS = 10000u;

static const auto SOURCE = R"txt(
Far far away, behind the word mountains, far from the countries Vokalia and
Consonantia, there live the blind texts. Separated they live in Bookmarksgrove right at the
coast of the Semantics, a large language ocean. A small river named Duden flows by
their place and supplies it with the necessary regelialia. It is a paradisematic
country, in which roasted parts of sentences fly into your mouth. Even the all-powerful
Pointing has no control about the blind texts it is an almost unorthographic life One
day however a small line of blind text by the name of Lorem Ipsum decided to leave
for the far World of Grammar. The Big Oxmox advised her not to do so, because
there were thousands of bad Commas, wild Question Marks and devious Semikoli, but
the Little Blind Text didn’t listen. She packed her seven versalia, put her
initial into the belt and made herself on the way. When she reached the first hills of
the Italic Mountains, she had a last view back on the skyline of her hometown
Bookmarksgrove, the headline of Alphabet Village and the subline of her own road, the Line
Lane. Pityful a rethoric question ran over her cheek, then she continued her way. On
her way she met a copy. The copy warned the Little Blind Text, that where it came
from it would have been rewritten a thousand times and everything that was left
from its origin would be the word "and" and the Little Blind Text should turn
around and return to its own, safe country.

But nothing the copy said could convince her and so it didn’t take long until
a few insidious Copy Writers ambushed her, made her drunk with Longe and Parole
and dragged her into their agency, where they abused her for their projects again
and again. And if she hasn’t been rewritten, then they are still using her. Far
far away, behind the word mountains, far from the countries Vokalia and
Consonantia, there live the blind texts. Separated they live in Bookmarksgrove right at the
coast of the Semantics, a large language ocean. A small river named Duden flows by
their place and supplies it with the necessary regelialia. It is a paradisematic
country, in which roasted parts of sentences fly into your mouth. Even the all-powerful
Pointing has no control about the blind texts it is an almost unorthographic life One
day however a small line of blind text by the name of Lorem Ipsum decided to leave
for the far World of Grammar. The Big Oxmox advised her not to do so, because
there were thousands of bad Commas, wild Question Marks and devious Semikoli, but
the Little Blind Text didn’t listen. She packed her seven versalia, put her
initial into the belt and made herself on the way. When she reached the first hills of
the Italic Mountains, she had a last view back on the skyline of her hometown
Bookmarksgrove, the headline of Alphabet Village and the subline of her own road, the Line
Lane.

Pityful a rethoric question ran over her cheek, then she continued her way. On
her way she met a copy. The copy warned the Little Blind Text, that where it came
from it would have been rewritten a thousand times and everything that was left
from its origin would be the word "and" and the Little Blind Text should turn
around and return to its own, safe country. But nothing the copy said could convince
her and so it didn’t take long until a few insidious Copy Writers ambushed her,
made her drunk with Longe and Parole and dragged her into their agency, where they
abused her for their projects again and again. And if she hasn’t been rewritten, then
they are still using her. Far far away, behind the word mountains, far from the
countries Vokalia and Consonantia, there live the blind texts. Separated they live in
Bookmarksgrove right at the coast of the Semantics, a large language ocean. A small river
named Duden flows by their place and supplies it with the necessary regelialia. It
is a paradisematic country, in which roasted parts of sentences fly into your
mouth. Even the all-powerful Pointing has no control about the blind texts it is an
almost unorthographic life One day however a small line of blind text by the name of
Lorem Ipsum decided to leave for the far World of Grammar. The Big Oxmox advised her
not to do so, because there were thousands of bad Commas, wild Question Marks and
devious Semikoli, but the Little Blind Text didn’t listen.
)txt"s;

static const auto RESULT = R"txt(
Fr fr wy, bhnd th wrd mntns, fr frm th cntrs Vkl nd
Cnsnnt, thr lv th blnd txts. Sprtd thy lv n Bkmrksgrv rght t th
cst f th Smntcs,  lrg lngg cn.  smll rvr nmd Ddn flws by
thr plc nd sppls t wth th ncssry rgll. t s  prdsmtc
cntry, n whch rstd prts f sntncs fly nt yr mth. vn th ll-pwrfl
Pntng hs n cntrl bt th blnd txts t s n lmst nrthgrphc lf n
dy hwvr  smll ln f blnd txt by th nm f Lrm psm dcdd t lv
fr th fr Wrld f Grmmr. Th Bg xmx dvsd hr nt t d s, bcs
thr wr thsnds f bd Cmms, wld Qstn Mrks nd dvs Smkl, bt
th Lttl Blnd Txt ddn’t lstn. Sh pckd hr svn vrsl, pt hr
ntl nt th blt nd md hrslf n th wy. Whn sh rchd th frst hlls f
th tlc Mntns, sh hd  lst vw bck n th skyln f hr hmtwn
Bkmrksgrv, th hdln f lphbt Vllg nd th sbln f hr wn rd, th Ln
Ln. Ptyfl  rthrc qstn rn vr hr chk, thn sh cntnd hr wy. n
hr wy sh mt  cpy. Th cpy wrnd th Lttl Blnd Txt, tht whr t cm
frm t wld hv bn rwrttn  thsnd tms nd vrythng tht ws lft
frm ts rgn wld b th wrd "nd" nd th Lttl Blnd Txt shld trn
rnd nd rtrn t ts wn, sf cntry.

Bt nthng th cpy sd cld cnvnc hr nd s t ddn’t tk lng ntl
 fw nsds Cpy Wrtrs mbshd hr, md hr drnk wth Lng nd Prl
nd drggd hr nt thr gncy, whr thy bsd hr fr thr prjcts gn
nd gn. nd f sh hsn’t bn rwrttn, thn thy r stll sng hr. Fr
fr wy, bhnd th wrd mntns, fr frm th cntrs Vkl nd
Cnsnnt, thr lv th blnd txts. Sprtd thy lv n Bkmrksgrv rght t th
cst f th Smntcs,  lrg lngg cn.  smll rvr nmd Ddn flws by
thr plc nd sppls t wth th ncssry rgll. t s  prdsmtc
cntry, n whch rstd prts f sntncs fly nt yr mth. vn th ll-pwrfl
Pntng hs n cntrl bt th blnd txts t s n lmst nrthgrphc lf n
dy hwvr  smll ln f blnd txt by th nm f Lrm psm dcdd t lv
fr th fr Wrld f Grmmr. Th Bg xmx dvsd hr nt t d s, bcs
thr wr thsnds f bd Cmms, wld Qstn Mrks nd dvs Smkl, bt
th Lttl Blnd Txt ddn’t lstn. Sh pckd hr svn vrsl, pt hr
ntl nt th blt nd md hrslf n th wy. Whn sh rchd th frst hlls f
th tlc Mntns, sh hd  lst vw bck n th skyln f hr hmtwn
Bkmrksgrv, th hdln f lphbt Vllg nd th sbln f hr wn rd, th Ln
Ln.

Ptyfl  rthrc qstn rn vr hr chk, thn sh cntnd hr wy. n
hr wy sh mt  cpy. Th cpy wrnd th Lttl Blnd Txt, tht whr t cm
frm t wld hv bn rwrttn  thsnd tms nd vrythng tht ws lft
frm ts rgn wld b th wrd "nd" nd th Lttl Blnd Txt shld trn
rnd nd rtrn t ts wn, sf cntry. Bt nthng th cpy sd cld cnvnc
hr nd s t ddn’t tk lng ntl  fw nsds Cpy Wrtrs mbshd hr,
md hr drnk wth Lng nd Prl nd drggd hr nt thr gncy, whr thy
bsd hr fr thr prjcts gn nd gn. nd f sh hsn’t bn rwrttn, thn
thy r stll sng hr. Fr fr wy, bhnd th wrd mntns, fr frm th
cntrs Vkl nd Cnsnnt, thr lv th blnd txts. Sprtd thy lv n
Bkmrksgrv rght t th cst f th Smntcs,  lrg lngg cn.  smll rvr
nmd Ddn flws by thr plc nd sppls t wth th ncssry rgll. t
s  prdsmtc cntry, n whch rstd prts f sntncs fly nt yr
mth. vn th ll-pwrfl Pntng hs n cntrl bt th blnd txts t s n
lmst nrthgrphc lf n dy hwvr  smll ln f blnd txt by th nm f
Lrm psm dcdd t lv fr th fr Wrld f Grmmr. Th Bg xmx dvsd hr
nt t d s, bcs thr wr thsnds f bd Cmms, wld Qstn Mrks nd
dvs Smkl, bt th Lttl Blnd Txt ddn’t lstn.
)txt"s;

constexpr auto VOWELS = { 'a','e','i','o','u','A','E','I','O','U' };

} // namespace consts


void copy( std::istream& is, std::ostream& os )
{
     celero::DoNotOptimizeAway( std::copy(
          std::istreambuf_iterator< char >{ is }
          , std::istreambuf_iterator< char >{}
          , std::ostreambuf_iterator< char >{ os }
          ) );
}


} // namespace bm_env
} // namespace {unnamed}



BASELINE( CharRemover, CopyStream, bm_env::consts::N_SAMPLES, bm_env::consts::N_ITERATIONS )
{
     std::istringstream is{ bm_env::consts::SOURCE };
     std::ostringstream os;

     bm_env::copy( is, os );

     BOOST_ASSERT( os.str() == bm_env::consts::SOURCE );
}


BENCHMARK( CharRemover, CharFilter, bm_env::consts::N_SAMPLES, bm_env::consts::N_ITERATIONS )
{
     std::istringstream is{ bm_env::consts::SOURCE };
     std::ostringstream os;

     using_boost::iostreams::filters::single_char::CharRemover vr{ bm_env::consts::VOWELS };;

     boost::iostreams::filtering_istream fis;
     fis.push( boost::ref( vr ) );
     fis.push( is );

     bm_env::copy( fis, os );

     BOOST_ASSERT( os.str() == bm_env::consts::RESULT );
}


BENCHMARK( CharRemover, BlockFilter, bm_env::consts::N_SAMPLES, bm_env::consts::N_ITERATIONS )
{
     std::istringstream is{ bm_env::consts::SOURCE };
     std::ostringstream os;

     using_boost::iostreams::filters::multichar::CharRemover vr{ bm_env::consts::VOWELS };;

     boost::iostreams::filtering_istream fis;
     fis.push( boost::ref( vr ) );
     fis.push( is );

     bm_env::copy( fis, os );

     BOOST_ASSERT( os.str() == bm_env::consts::RESULT );
}


BENCHMARK( CharRemover, CharFilterBoost, bm_env::consts::N_SAMPLES, bm_env::consts::N_ITERATIONS )
{
     boost::iostreams::stream< boost::iostreams::array_source > is{
          boost::iostreams::array_source{
               bm_env::consts::SOURCE.data(), bm_env::consts::SOURCE.size()
          }
     };
     std::ostringstream os;

     using_boost::iostreams::filters::single_char::CharRemover vr{ bm_env::consts::VOWELS };;

     boost::iostreams::filtering_istream fis;
     fis.push( boost::ref( vr ) );
     fis.push( is );

     bm_env::copy( fis, os );

     BOOST_ASSERT( os.str() == bm_env::consts::RESULT );
}


BENCHMARK( CharRemover, BlockFilterBoost, bm_env::consts::N_SAMPLES, bm_env::consts::N_ITERATIONS )
{
     boost::iostreams::stream< boost::iostreams::array_source > is{
          boost::iostreams::array_source{
               bm_env::consts::SOURCE.data(), bm_env::consts::SOURCE.size()
          }
     };
     std::ostringstream os;

     using_boost::iostreams::filters::multichar::CharRemover vr{ bm_env::consts::VOWELS };;

     boost::iostreams::filtering_istream fis;
     fis.push( boost::ref( vr ) );
     fis.push( is );

     bm_env::copy( fis, os );

     BOOST_ASSERT( os.str() == bm_env::consts::RESULT );
}
