(function($) {
  $('link').each(function() {
    if ($(this).attr('href').match(/\.less$/i) != null) {
      $(this).attr('rel', 'stylesheet/less');
      var href = $(this).attr('href');
      $(this).attr('href', '/' + href);
    }
  });
})(jQuery);
