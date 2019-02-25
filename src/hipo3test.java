import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import org.jlab.clas.physics.LorentzVector;
import org.jlab.groot.data.DataLine;
import org.jlab.groot.data.H1F;

import org.jlab.groot.ui.TCanvas;
import org.jlab.jnp.hipo.data.HipoEvent;
import org.jlab.jnp.hipo.data.HipoGroup;
import org.jlab.jnp.hipo.io.HipoReader;

import org.jlab.groot.base.GStyle;
import org.jlab.groot.data.H2F;

import org.apache.commons.lang.time.StopWatch;

import java.io.IOException;
import java.nio.file.*;

import java.util.regex.Matcher;
import java.util.regex.Pattern;
import java.util.stream.Stream;

import static java.lang.Float.NaN;

public class hipo3test {
	public static Logger LOGGER = LogManager.getLogger(hipo3test.class.getName());
	public static double beamEnergy = 6.42313;
	public static double MASS_P = 0.93827203;
	public static double MASS_PIP = 0.13957018;
	public static double c_special_units = 29.9792458;

	public final static LorentzVector e_mu = new LorentzVector(0.0,0.0, beamEnergy, beamEnergy);
	public final static LorentzVector p_mu = new LorentzVector(0.0,0.0,0.0, MASS_P);

	public static int eventNumber = 0;
	public static StopWatch watch = new StopWatch();

	public static H1F w = new H1F("hist", 500, 0, 4.0);
	public static H1F w_cut = new H1F("hist", 250, 0, 4.0);
	public static H2F wq2_elec = new H2F("hist", 500, 0, 4.0, 500, 0, 2.5);
	public static H2F w_mm = new H2F("hist", 500, 0, 4.0, 500, 0, 3);
	public static H1F missingMass = new H1F("MissingMass",200,0,5.5);
	public static H1F missingMass_cut = new H1F("MissingMass",200,0,5.5);
	public static H2F sf_elec = new H2F("sf_elec",200,0,7.5,200,0.0,0.5);
	public static H2F dt_pip_hist = new H2F("sf_elec",200,0,5.5,200,-5.0,5.0);


	static Stream glob(String mask) {
		Matcher matcher = Pattern.compile("(.*)[\\\\/](.*)").matcher(mask);
		Path parentPath;
		String fName;
		if(matcher.matches()) {
			parentPath = Paths.get(matcher.group(1));
			fName = matcher.group(2);
		} else {
			parentPath = Paths.get(".");
			fName = mask;
		}
		PathMatcher pathMatcher = FileSystems.getDefault().getPathMatcher("glob:" + fName);
		try {
			return Files.list(parentPath).filter(p->pathMatcher.matches(p.getFileName()));
		} catch (IOException te) {
			LOGGER.error(te);
			System.exit(1);
			return null;
		}
	}

	public static double vertex_time(double sc_time, double sc_pathlength, double relatavistic_b) {
		return sc_time - sc_pathlength / (relatavistic_b * c_special_units);
	}

	public static double delta_t_calc(double vertex, double momentum, double sc_time, double sc_pathlength, double mass) {
		double b = 1.0 / Math.sqrt(1.0 + (mass / momentum) * (mass / momentum));
		double dt = vertex - vertex_time(sc_time, sc_pathlength, b);
		return dt;
	}

	public static double W(LorentzVector e_mu_prime){
		LorentzVector q_mu = new LorentzVector(e_mu);
		q_mu.sub(e_mu_prime);
		LorentzVector _p_mu = new LorentzVector(p_mu);
		_p_mu.add(q_mu);
		return _p_mu.mass();
	}

	public static double miss(LorentzVector e_mu_prime, LorentzVector pionP){
		LorentzVector q_mu = new LorentzVector(e_mu);
		q_mu.sub(e_mu_prime);
		LorentzVector _p_mu = new LorentzVector(p_mu);
		_p_mu.add(q_mu);
		_p_mu.sub(pionP);
		return _p_mu.mass();
	}

	public static double Q2(LorentzVector e_mu_prime){
		LorentzVector q_mu = new LorentzVector(e_mu);
		q_mu.sub(e_mu_prime);
		return -q_mu.mass2();
	}

	public static void process(String filename){
		HipoReader reader = new HipoReader();
		reader.open(filename);

		while(reader.hasNext()) {
			HipoEvent event = reader.readNextEvent();

			if (!event.hasGroup("REC::Particle")) continue;
			HipoGroup particles = event.getGroup("REC::Particle");

			if (!event.hasGroup("REC::Calorimeter")) continue;
			HipoGroup calos = event.getGroup("REC::Calorimeter");

			if (!event.hasGroup("REC::Scintillator")) continue;
			HipoGroup scint = event.getGroup("REC::Scintillator");

			if(particles.getNode( "charge").getByte(0) != -1) continue;
			eventNumber++;
			LorentzVector e_mu_prime = new LorentzVector();
			e_mu_prime.setPxPyPzM(particles.getNode("px").getFloat( 0), particles.getNode("py").getFloat( 0), particles.getNode("pz").getFloat( 0), 0.000511);

			double energy = 0;
			for (int icalo = 0; icalo < calos.getMaxSize(); icalo++) {
				if (calos.getNode("pindex").getShort( icalo) == 0) {
					if (calos.getNode("detector").getByte( icalo) == 7) {
						energy += calos.getNode("energy").getFloat(icalo);
					}
				}
			}

			if (energy != 0) {
				sf_elec.fill(e_mu_prime.p(), energy / e_mu_prime.p());
			}

			if ((energy / e_mu_prime.p()) < 0.2 || (energy / e_mu_prime.p()) >0.3) continue;
			w.fill(W(e_mu_prime));
			wq2_elec.fill(W(e_mu_prime), Q2(e_mu_prime));
			double e_vertex = NaN;
			for (int iscint = 0; iscint < scint.getMaxSize(); iscint++) {
				if (scint.getNode("pindex").getShort(iscint) == 0) {
					if (scint.getNode("detector").getByte(iscint) == 12) {
						if (scint.getNode("layer").getByte(iscint) == 2) {
							double sc_t = scint.getNode("time").getFloat(iscint);
							double sc_r = scint.getNode("path").getFloat(iscint);
							e_vertex = vertex_time(sc_t, sc_r, 1.0);
							break;
						}
					}
				}
			}

			LorentzVector pionP = new LorentzVector();

			int numpip = 0;
			int numOther = 0;
			for (int part = 1; part < particles.getMaxSize(); part++) {
				pionP.setPxPyPzM(particles.getNode("px").getFloat(part),particles.getNode("py").getFloat(part),particles.getNode("pz").getFloat(part),MASS_PIP);

				double pip_dt = NaN;
				if(particles.getNode("charge").getByte(part) == 1) {
					for (int iscint = 0; iscint < scint.getMaxSize(); iscint++) {
						if (scint.getNode("pindex").getShort(iscint) == part) {
							if (scint.getNode("detector").getByte(iscint) == 12) {
								if (scint.getNode("layer").getByte(iscint) == 2) {
									double sc_t = scint.getNode("time").getFloat(iscint);
									double sc_r = scint.getNode("path").getFloat(iscint);
									pip_dt = delta_t_calc(e_vertex, pionP.p(), sc_t, sc_r, MASS_PIP);
									dt_pip_hist.fill(pionP.p(),pip_dt);
								}
							}
						}
					}
				}

				if(Math.abs(pip_dt) < 0.5 && particles.getNode("charge").getByte(part) == 1) {
					numpip++;
				} else{
					numOther++;
				}
			}

			if(numpip == 1 && numOther == 0) {
				missingMass.fill(miss(e_mu_prime,pionP));
				w_mm.fill(W(e_mu_prime),miss(e_mu_prime,pionP));
				if(miss(e_mu_prime,pionP) >= 0.8 && miss(e_mu_prime,pionP) <= 1.1) {
					missingMass_cut.fill(miss(e_mu_prime,pionP));
					w_cut.fill(W(e_mu_prime));
				}

			}
		}

		LOGGER.info(eventNumber + " Events in " + watch.getTime() * 0.001 + " sec");
		LOGGER.info(eventNumber / (watch.getTime() * 0.001) + " Hz");
	}


	public static void main(String[] args) {
		GStyle.setPalette("kViridis");
		GStyle.getAxisAttributesX().setTitleFontSize(16);
		GStyle.getAxisAttributesY().setTitleFontSize(16);
		GStyle.getAxisAttributesX().setLabelFontSize(16);
		GStyle.getAxisAttributesY().setLabelFontSize(16);
		GStyle.getAxisAttributesZ().setLabelFontSize(16);

		String path;
		if(args.length == 2) {
			path = args[0];
			beamEnergy = Double.parseDouble(args[1]);
			e_mu.setPxPyPzE(0.0,0.0,beamEnergy,beamEnergy);
			LOGGER.debug(path);
			LOGGER.debug(beamEnergy);
		} else {
			path = "/Users/tylern/Data/hipo/skim4_*.hipo";
		}

		Stream files= glob(path);

		TCanvas can = new TCanvas("W vs Q^2", 1200, 800);
		can.getCanvas().initTimer(1000);

		can.getPad().setOptStat(11111);
		can.divide(3,2);
		w.setTitleX("W (GeV)");

		wq2_elec.setTitleX("W (GeV)");
		wq2_elec.setTitleY("Q^2 (GeV^2)");

		missingMass.setTitleX("Mass (GeV)");

		sf_elec.setTitleX("Momentum  (GeV)");
		sf_elec.setTitleY("Sampling Fraction");

		dt_pip_hist.setTitleX("Momentum  (GeV)");
		dt_pip_hist.setTitleY("#Delta T");

		w_cut.setTitleX("W (GeV)");

		can.cd(0);
		can.draw(sf_elec);
		DataLine sf_lint_t = new DataLine(0,0.2,7.5,0.2);
		sf_lint_t.setLineColor(44);
		sf_lint_t.setLineWidth(4);
		sf_lint_t.setLineStyle(2);
		can.draw(sf_lint_t);
		DataLine sf_lint_b = new DataLine(0,0.3,7.5,0.3);
		sf_lint_b.setLineColor(44);
		sf_lint_b.setLineWidth(4);
		sf_lint_b.setLineStyle(2);
		can.draw(sf_lint_b);
		can.cd(1);
		can.draw(wq2_elec);
		can.cd(2);
		can.draw(w);
		can.cd(3);
		can.draw(dt_pip_hist);
		DataLine lint_t = new DataLine(0,0.5,5.5,0.5);
		lint_t.setLineColor(44);
		lint_t.setLineWidth(4);
		lint_t.setLineStyle(2);
		can.draw(lint_t);
		DataLine lint_b = new DataLine(0,-0.5,5.5,-0.5);
		lint_b.setLineColor(44);
		lint_b.setLineWidth(4);
		lint_b.setLineStyle(2);
		can.draw(lint_b);
		can.cd(4);
		can.draw(missingMass);
		missingMass_cut.setFillColor(44);
		can.draw(missingMass_cut,"same");
		can.cd(5);
		w_cut.setFillColor(44);
		can.draw(w_cut);

		watch.start();
		files.forEach(fil->process(fil.toString()));
		watch.stop();

		LOGGER.info(eventNumber + " Events in " + watch.getTime() * 0.001 + " sec");
		LOGGER.info(eventNumber / (watch.getTime() * 0.001) + " Hz");


		can.save("hipo_results.png");
		System.exit(0);
	}

}
